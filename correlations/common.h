#pragma once
#include <iostream>
#include <sstream>
#include "icsv.h"
#include <map>
#include <algorithm>
#include <iterator>
#include <functional>
#include <cmath>

#define _CMN ::common::

constexpr double standard_deviation_err_value = -1;

namespace common {
    template<typename Stream>
    char getline(Stream &input, std::string &str, char delim_1, char delim_2);
    template<typename Stream>
    char ignore(Stream &input, std::streamsize count, char delim_1, char delim_2);

    template<typename T>
    class unique_counter {
        std::map<T, bool> values{};
    public:
        void operator()(const T &t);
        [[nodiscard]] std::size_t quantity() const;
    };

    template<typename Iter>
    [[nodiscard]] std::size_t count_unique(Iter begin, Iter end);

    template<typename Iter>
    [[nodiscard]] Iter median(Iter begin, Iter end);

    template<typename Iter>
    double mean(Iter begin, Iter end);
    template<typename Iter>
    double standard_deviation(Iter begin, Iter end, double mean);
}

template<typename Stream>
char common::getline(Stream &input, std::string &str, char delim_1, char delim_2) {
    str.erase();
    char c;
    while (input.good() && str.size() < str.max_size()) {
        c = input.get();
        if (c == delim_1 || c == delim_2 ||
            c == _STD char_traits<char>::eof()) {
            return c;
        }
        str.append(1,c);
    }
    return input.eof()
           ? _STD char_traits<char>::eof()
           : '\0';
}

template<typename Stream>
char common::ignore(Stream &input, std::streamsize count, char delim_1, char delim_2) {
    char c;
    while (input.good() && 0 < count) {
        if (count != std::numeric_limits<std::streamsize>::max()
            && --count < 0) {
            return '\0';//считано необходимое количество символов
        } else {
            c = input.get();//считываем символ
            if (c == delim_1 || c == delim_2 ||
                c == _STD char_traits<char>::eof()) {
                return c;//заканчиваем, если считан один из разделителей
            }
        }
    }
    return input.eof()
           ? _STD char_traits<char>::eof()
           : '\0';
}

template<typename T>
void common::unique_counter<T>::operator()(const T &t) {
    auto iter = values.find(t);
    if (iter == values.end()) {//t попадается первый раз
        values.insert(_STD make_pair(t, true));//true means unique
    } else {//t попадается не в первый раз
        iter->second = false;//false means not unique
    }
}

template<typename T>
std::size_t common::unique_counter<T>::quantity() const {
    std::size_t count = 0;
    std::for_each(values.cbegin(), values.cend(),
            [&count](const std::pair<T, bool> &p) {
                if (p.second) {++count;}
            }
    );
    return count;
}

template<typename Iter>
std::size_t common::count_unique(Iter begin, Iter end) {
    return _STD for_each(begin, end, _CMN unique_counter<typename Iter::value_type>()).quantity();
}

template<typename Iter>
Iter common::median(Iter begin, Iter end) {
    return _STD next(begin, (_STD distance(begin, end) - 1)/2);
}

template<typename Iter>
double common::mean(Iter begin, Iter end) {
    double result = 0;
    _STD size_t n = 0;
    _STD for_each(begin, end,
            [&result, n](const typename Iter::value_type &x) mutable {
                result *= double(n)/(n+1);
                result += (1/double(n+1))*x;
                ++n;
            });
    return result;
}

template<typename Iter>
double common::standard_deviation(Iter begin, Iter end, double mean) {
    if (_STD distance(begin, end) < 2) {
        return ::standard_deviation_err_value;
    }//begin, (begin + 1) могут быть разыменованы, (begin + 2) существует

    using T = typename Iter::value_type;
    auto sq = [mean](const T &x) {
        return (x - mean)*(x - mean);
    };

    double result = sq(*begin) + sq(*_STD next(begin));
    _STD advance(begin, 2);
    _STD size_t n = 2;

    _STD for_each(begin, end,
            [&result, n, sq](const T &x) mutable {
                result *= double(n-1)/n;
                result += (1/double(n))*sq(x);
                ++n;
            });
    return _STD sqrt(result);
}
