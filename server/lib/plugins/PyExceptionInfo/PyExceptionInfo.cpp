#include "PyExceptionInfo.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include <boost/python/exec.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/import.hpp>
#include <boost/python/object_fwd.hpp>

auto PyExceptionInfo::build() -> std::optional<PyExceptionInfo> {
    // PyErr_Print обязатетен
    // https://stackoverflow.com/a/57896281
    PyExceptionInfo info;

    try {
        PyErr_Print();
        boost::python::object main_namespace =
            boost::python::import("__main__").attr("__dict__");

        exec("import traceback, sys", main_namespace);
        boost::python::object py_last_type =
            eval("str(sys.last_type)", main_namespace);
        boost::python::object py_last_value =
            eval("str(sys.last_value)", main_namespace);
        boost::python::object py_last_traceback =
            eval("str(sys.last_traceback)", main_namespace);
        boost::python::object py_stack_trace =
            eval("'\\n'.join(traceback.format_exception(sys.last_type, "
                 "sys.last_value, sys.last_traceback))",
                 main_namespace);

        info.last_type_  = boost::python::extract<std::string>(py_last_type);
        info.last_value_ = boost::python::extract<std::string>(py_last_value);
        info.last_traceback_ =
            boost::python::extract<std::string>(py_last_traceback);
        info.stack_trace_ = boost::python::extract<std::string>(py_stack_trace);

        PyErr_Clear();
    } catch (const boost::python::error_already_set &) {
        SPDLOG_ERROR("Couldn't extract python exception info");
        return std::nullopt;
    }
    return info;
}
