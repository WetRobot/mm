#ifndef ITERATOR_HPP
#define ITERATOR_HPP

namespace it {
    template <typename Iterator>
    class SumIterable {
        public:
            Iterator begin;
            Iterator end;
            SumIterable(Iterator begin, Iterator end) : begin(begin), end(end) {};
    };
}

namespace it {
    template <typename Iterator>
    class ByIterable {
        public:
            Iterator begin;
            Iterator end;
            ByIterable(Iterator begin, Iterator end) : begin(begin), end(end) {};
    };
}

#endif
