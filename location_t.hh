#pragma once

#include <cstddef>
#include <ostream>
#include <utility>

namespace foo
{
    struct loc_type {
        int first_line;
        int first_column;
        int last_line;
        int last_column;
    };
    using namespace std;
    using position_t = size_t;
    // using location_t = tuple<size_t, size_t, size_t, size_t>;
    using location_t = loc_type;
}

inline std::ostream& operator<<(std::ostream& os, const foo::location_t& loc)
{
    return os << "[" << loc.first_line << ":" << loc.first_column 
            << " - " << loc.last_line << ":" << loc.last_column << "]";
}