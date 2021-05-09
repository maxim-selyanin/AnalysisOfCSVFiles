#pragma once
#include "stats.h"

#define SORT_FIRST
//#define ALERTS

#ifdef ALERTS
void alert(const char *text = nullptr) {
    if (text) {
        std::cout << text;
    } else {
        std::cout << "in action";
    }
    std::cout << '\n';
}
#else
#define alert(n) void(n)
#endif

template<typename T, std::size_t N>
struct table {
    static constexpr bool nc = ::non_categorical<T>;
    static constexpr std::size_t capacity = (nc) ? 2*N : N;//на случай первых разностей некат. выборок
    ::sample<T> data[capacity];

    void fill(::icsv &file, std::size_t start_column);
    void sort();
    void out(_SIT &s) const;
};

template<typename T, std::size_t N>
void table<T,N>::fill(::icsv &file, std::size_t start_column) {
    if constexpr (nc) {
        for (std::size_t i = 0; i < capacity; i += 2) {
            data[i] = ::get_sample<T>(file, start_column++);
#ifdef SORT_FIRST
            data[i].sort();
#endif

            data[i + 1] = data[i].first_differences();
#ifdef SORT_FIRST
            data[i + 1].sort();
#endif
        }
    } else {
        for (std::size_t i = 0; i < capacity; ++i) {
            data[i] = ::get_sample<T>(file, start_column++);
        }
    }
}

template<typename T, std::size_t N>
void table<T, N>::sort() {
#ifndef SORT_FIRST
    for (std::size_t i = 0; i < capacity; ++i) {
        data[i].sort();
    }
#endif
}

template<typename T, std::size_t N>
void table<T, N>::out(string_iteration &s) const {
    ::features::data<T> d;
    for (std::size_t i = 0; i < capacity; ++i) {
        d.name = data[i].name;
        d.q = data[i].get_quantitative();
        d.s = data[i].get_stat();
        d.write_to(s);
    }
}
