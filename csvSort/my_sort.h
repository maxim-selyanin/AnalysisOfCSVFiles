#pragma once

template<typename Iter>
void move_forward(Iter begin, Iter end) {
    //гарантируется, что end - действительный итератор, то есть next(end) существует
    std::reverse_iterator<Iter> rbegin(std::next(end));
    std::reverse_iterator<Iter> rend(std::next(begin));
    for (auto i = rbegin; i < rend; ++i) {
        i[0] = std::move(i[1]);
    }
}

template<typename Iter>
Iter find_position(Iter begin, Iter end, typename Iter::value_type element) {
    std::reverse_iterator<Iter> rbegin(end);
    std::reverse_iterator<Iter> rend(begin);
    for (auto i = rbegin; i < rend; ++i) {
        if (! _STD greater<typename Iter::value_type>()(*i, element)) {
            return i.base();
        }
    }
    return begin;
}

template<typename Iter>
void sort(Iter begin, Iter end) {
    typename Iter::value_type tmp;
    Iter position;

    for (auto i = begin; i < end; ++i) {
        tmp = std::move(*i);
        position = ::find_position(begin, i, tmp);//находим, куда вставлять tmp
        ::move_forward(position, i);//освобождаем место для tmp
        *position = std::move(tmp);
    }
}