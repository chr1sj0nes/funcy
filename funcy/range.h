#ifndef FUNCY_RANGE_H
#define FUNCY_RANGE_H

#include "iter.h"

template <typename Idx>
class RangeIterator : public Iterator<RangeIterator<Idx>, Idx> {
    Idx pos;
    const Idx end;

public:
    RangeIterator(Idx start, Idx end) : pos(start), end(end) {}

    using Item = Idx;

    Option<Item> next() { if (pos != end) return pos++; else return None; }
};

template <typename Idx>
RangeIterator<Idx> range(Idx start, Idx end) { return {start, end}; }

#endif //FUNCY_RANGE_H
