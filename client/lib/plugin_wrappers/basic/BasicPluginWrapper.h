#ifndef BASICPLUGINWRAPPER_H
#define BASICPLUGINWRAPPER_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "IBasicPluginWrapper.h"
#include "IRequestable.h"

class BasicPluginWrapper : virtual public IBasicPluginWrapper {
 public:
    BasicPluginWrapper(std::shared_ptr<IRequestable> connection,
                       std::string                   plugin_type);
    std::string init(const std::string &plugin_name) override;
    std::pair<std::string, std::string> get_default_config() override;
    std::pair<std::string, std::string> get_default_scheme() override;
    std::pair<std::map<std::string, std::string>, std::string>
    set_config(const std::string &new_config) override;
    std::pair<LoadResult, std::string> list_plugins() override;
    std::pair<LoadResult, std::string> load_new_plugins() override;

 protected:
    std::shared_ptr<IRequestable> connection_;
    std::string                   plugin_type_;
};

#endif  // BASICPLUGINWRAPPER_H
