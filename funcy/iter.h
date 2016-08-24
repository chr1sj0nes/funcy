#ifndef FUNCY_ITER_H
#define FUNCY_ITER_H

#include <vector>
#include "option.h"

#include <iostream>


template <typename InIter, typename UnaryOp>
class Map;

template <typename InIter, typename State, typename BinaryOp>
class Scan;

template <typename InIter, typename UnaryPredicate>
class Filter;

template <typename Self, typename Item>
class Iterator {
    Self& self() { return *static_cast<Self*>(this); }

public:
    template <typename UnaryOp>
    Map<Self, UnaryOp> map(UnaryOp f) { return {self(), f}; }

    template <typename State, typename BinaryOp>
    Scan<Self, BinaryOp, std::decay_t<State>> scan(State&& s, BinaryOp f) {
        return {self(), f, std::forward<State>(s)};
    };

    template <typename UnaryPredicate>
    Filter<Self, UnaryPredicate> filter(UnaryPredicate f) { return {self(), f}; }

    template <typename Item2, typename BinaryOp>
    Item2 fold(Item2 acc, BinaryOp f) {
        while (Option<Item> it = self().next()) acc = f(acc, *it);
        return acc;
    };

    template <typename UnaryPredicate>
    bool all(UnaryPredicate f) {
        while (Option<Item> it = self().next()) {
            if (!f(*it)) return false;
        }
        return true;
    };

    template <typename It = Item>
    std::enable_if_t<std::is_convertible<It, bool>::value, bool> all() {
        return all([](auto a) { return a; });
    }

    template <typename UnaryPredicate>
    bool any(UnaryPredicate f) {
        while (Option<Item> it = self().next()) {
            if (f(*it)) return true;
        }
        return false;
    };

    template <typename It = Item>
    std::enable_if_t<std::is_convertible<It, bool>::value, bool> any() {
        return any([](auto a) { return a; });
    }

    template <typename It = Item>
    std::enable_if_t<std::is_arithmetic<It>::value, It> sum() {
        return fold(0, std::plus<It>());
    }

    template <typename It = Item>
    std::enable_if_t<std::is_arithmetic<It>::value, It> product() {
        return fold(1, std::multiplies<It>());
    }

    auto enumerate() {
        return scan(0, [](size_t& i, auto v) { return std::make_pair(i++, v); });
    }

    // TODO reserve capacity for vectors
    template <typename Collection = std::vector<std::decay_t<Item>>>
    Collection collect() {
        Collection c;
        into(std::back_inserter(c));
        return c;
    }

    template <typename OutputIterator>
    void into(OutputIterator dst) {
        while (Option<Item> it = self().next()) {
            std::cout << "it = " << *it << std::endl;
            *(dst++) = *it;
        }
    }
};

template <typename Iterator>
using ItRef = typename std::iterator_traits<Iterator>::reference;

template <typename InputIterator>
class StdIterWrapper : public Iterator<StdIterWrapper<InputIterator>, ItRef<InputIterator>> {
    InputIterator pos;
    const InputIterator end;

public:
    StdIterWrapper(InputIterator start, InputIterator end) : pos(start), end(end) {}

    template <typename Collection>
    explicit StdIterWrapper(Collection c) : StdIterWrapper(c.cbegin(), c.cend()) {}

    using Item = ItRef<InputIterator>;

    Option<Item> next() {
        std::cout << "xxx = " << *pos << std::endl;
        if (pos != end) return *(pos++); else return None; }
};

template <typename Collection>
StdIterWrapper<typename Collection::const_iterator> iter(Collection c) {
    return StdIterWrapper<typename Collection::const_iterator>(c);
}

template <typename Item, size_t N>
StdIterWrapper<const Item*> iter(Item(&arr)[N]) {
    return StdIterWrapper<const Item*>(arr, arr + N);
}

template <typename InIter, typename UnaryOp>
using MapItem = std::result_of_t<UnaryOp(typename InIter::Item)>;

template <typename InIter, typename UnaryOp>
class Map : public Iterator<Map<InIter, UnaryOp>, MapItem<InIter, UnaryOp>> {
    InIter& i;
    const UnaryOp f;

public:
    Map(InIter& i, UnaryOp f) : i(i), f(f) {}

    using Item = MapItem<InIter, UnaryOp>;

    Option<Item> next() { return i.next().map(f); }
};

template <typename InIter, typename BinaryOp, typename State>
using ScanItem = std::result_of_t<BinaryOp(State&, typename InIter::Item)>;

template <typename InIter, typename BinaryOp, typename State>
class Scan : public Iterator<Scan<InIter, BinaryOp, State>, ScanItem<InIter, BinaryOp, State>> {
    InIter& i;
    const BinaryOp f;
    State s;

public:
    Scan(InIter& i, BinaryOp f, State&& s) : i(i), f(f), s(s) {}

    using Item = ScanItem<InIter, BinaryOp, State>;

    Option<Item> next() { return i.next().map([s](auto& a) { return f(s, a); }); }
};

template <typename InIter, typename UnaryPredicate>
class Filter : public Iterator<Filter<InIter, UnaryPredicate>, typename InIter::Item> {
    InIter& i;
    const UnaryPredicate f;

public:
    Filter(InIter& i, UnaryPredicate f) : i(i), f(f) {}

    using Item = typename InIter::Item;

    Option<Item> next() {
        while (Option<Item> it = i.next()) {
            if (f(*it)) return *it;
        }
        return None;
    }
};

#endif //FUNCY_ITER_H
