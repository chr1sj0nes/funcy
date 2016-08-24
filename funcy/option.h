#ifndef FUNCY_OPTION_H
#define FUNCY_OPTION_H

#include <boost/optional.hpp>

// TODO use C++17 implementations if available
using None_t = boost::none_t;
const None_t None = boost::none;

template <typename T>
class Option {
//    using raw_type       = std::remove_reference_t<T>;
//    using reference_type = raw_type&;
//    using pointer_type   = raw_type*;

    boost::optional<T> impl;

public:
    Option(reference_const_type val) : impl(val) {}
//    Option(T&& val) : impl(std::move(val)) {} // FIXME
    Option(None_t _) : impl(None) {}

    constexpr explicit operator bool() const { return impl; }

    constexpr pointer_const_type operator->() const { return impl.operator->(); }
    constexpr pointer_type operator->() { return impl.operator->(); }
    constexpr reference_const_type operator*() const { return *impl; }
    constexpr reference_type operator*() { return *impl; }

private:
    template <typename UnaryOp>
    using R = std::result_of_t<UnaryOp(T&)>;

public:
    template <typename UnaryOp>
    Option<R<UnaryOp>> map(UnaryOp f) const { if (*this) return f(*this); else return None; }
};

#endif //FUNCY_OPTION_H
