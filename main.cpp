#include "funcy/funcy.h"
#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::vector<int> v{1, 2, 3, 4, 5, 6};

    std::cout << "v:" << std::endl;
    for (const auto& val : v) {
        std::cout << val << std::endl;
    }

    auto v2 = iter(v).map([](auto a){ return a * 2; }).collect();

    std::cout << "v2:" << std::endl;
    for (const auto& val : v2) {
        std::cout << val << std::endl;
    }

    auto v3 = iter(v).filter([](auto a) { return a > 0; }).collect();

    std::cout << "v3:" << std::endl;
    for (const auto& val : v3) {
        std::cout << val << std::endl;
    }

    std::cout << "sum = " << iter(v).fold(0, std::plus<int>()) << std::endl;

    std::vector<bool> vb{true, true, true};
    std::cout << "vb, all = " << iter(vb).all() << ", any = " << iter(vb).any() << std::endl;

    auto vr = range(0, 10).map([](auto a){ return a * a; }).collect();
    std::cout << "vr:" << std::endl;
    for (const auto& val : vr) {
        std::cout << val << std::endl;
    }

    auto vs = range(0, 10).scan(0, [](auto& acc, auto a){ acc += a; return acc; }).collect();
    std::cout << "vs:" << std::endl;
    for (const auto& val : vs) {
        std::cout << val << std::endl;
    }

    return 0;
}