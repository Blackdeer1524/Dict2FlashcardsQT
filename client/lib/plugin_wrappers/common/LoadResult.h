#ifndef LOADRESULT_H
#define LOADRESULT_H

#include <vector>
#include <string>

struct LoadResult {
    std::vector<std::string> success;
    std::vector<std::string> fail;
};

inline bool operator==(const LoadResult &lhs, const LoadResult &rhs) {
    return lhs.success == rhs.success && lhs.fail == rhs.fail;
}

#endif  // LOADRESULT_H
