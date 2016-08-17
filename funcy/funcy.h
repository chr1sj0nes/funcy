#ifndef FUNC_FUNC_H
#define FUNC_FUNC_H

#include "iter.h"
#include "range.h"

template <typename R, typename A, typename B>
std::function<R(B)> curry(const std::function<R(A, B)>& f, A a) {
    return [f, a](B b) { return f(a, b); };
}

template <typename R, typename A, typename B>
std::function<R(A, B)> uncurry(const std::function<std::function<R(B)>(A)>& f) {
    return [f](A a, B b) { return f(a)(b); };
}

#endif //FUNC_FUNC_H
