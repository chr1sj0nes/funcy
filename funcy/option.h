#ifndef FUNCY_OPTION_H
#define FUNCY_OPTION_H

#include <boost/optional.hpp>

// TODO use C++17 implementations if available
template <typename T>
using Option = boost::optional<T>;
const boost::none_t None = boost::none;

#endif //FUNCY_OPTION_H
