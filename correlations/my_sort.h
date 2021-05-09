#pragma once
#include <iterator>

template<typename Iter>
void move_forward(Iter begin, Iter end) {
    //гарантируется, что end - действительный итератор, то есть next(end) существует
    _STD reverse_iterator<Iter> rbegin(_STD next(end));
    _STD reverse_iterator<Iter> rend(_STD next(begin));
    while (rbegin < rend) {
        *rbegin = _STD move(
                    * _STD next(rbegin++)
                    );
    }
}

template<typename Iter>
Iter find_position(Iter begin, Iter end, const typename Iter::value_type &element) {
    _STD reverse_iterator<Iter> rbegin(end);
    _STD reverse_iterator<Iter> rend(begin);
    while (rbegin < rend) {
        if (! _STD greater<typename Iter::value_type>()(*rbegin, element)) {
            return rbegin.base();
        }
        ++rbegin;
    }
    return begin;
}

template<typename Iter>
void sort(Iter begin, Iter end) {
    typename Iter::value_type tmp;
    Iter position;

    for (auto i = begin; i < end; ++i) {
        tmp = _STD move(*i);
        position = ::find_position(begin, i, tmp);//находим, куда вставлять tmp
        ::move_forward(position, i);//освобождаем место для tmp
        *position = _STD move(tmp);
    }
}
