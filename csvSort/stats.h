#pragma once
#include <limits>
#include <vector>
#include <algorithm>
#include "string_iteration.h"
#include "my_sort.h"

//#define MY_SORT

template<typename T>
constexpr bool non_cat_int = std::is_integral_v<T> && !std::is_same_v<T, bool>;
template<typename T>
constexpr bool non_categorical = ::non_cat_int<T> || std::is_floating_point_v<T>;
template<typename T>
using enable_noncat = _STD enable_if_t<::non_categorical<T>>;

namespace features {
    constexpr double standard_deviation_err_value = -1;
    constexpr double invalid_stats_standard_deviation = -2;

    struct quantitative {
        std::size_t quantity{0};
        std::size_t empty{0};
        std::size_t unique{1};

        [[nodiscard]] bool invalid() const {return unique > quantity;}

        void write_to(_SIT &s) const {
            if (this->invalid()) {
                for (int i = 0; i < 3; ++i) {
                    s.suppl_row_newline(_SIT::empty());
                }
                return;
            }

            s.suppl_row_newline(quantity, _SIT::raw);
            s.suppl_row_newline(empty, _SIT::raw);
            s.suppl_row_newline(unique, _SIT::raw);
        }
    };

    template<typename T>
    struct stat {
        T min;
        T max;
        T median;
        T quartile_range;
        double mean;
        double standard_deviation{invalid_stats_standard_deviation};

        [[nodiscard]] bool invalid() const {
            return standard_deviation == invalid_stats_standard_deviation;}

        void write_to(_SIT &s) const;
    };

    template<typename T>
    struct data {
        std::string name{};
        ::features::quantitative q{};
        ::features::stat<T> s{};

        void write_to(_SIT &str) const {
            str.suppl_row_newline<_STD string>(name);
            q.write_to(str);
            s.write_to(str);
            str.seek_start();
        }
    };
}//namespace features

template<typename T>
void features::stat<T>::write_to(_SIT &s) const {
    if (this->invalid()) {
        for (int i = 0; i < 6; ++i) {
            s.suppl_row_newline(_SIT::empty());
        }
        return;
    }

    s.suppl_row_newline(this->min, _SIT::raw);
    s.suppl_row_newline(this->max, _SIT::raw);
    s.suppl_row_newline(this->median, _SIT::raw);
    s.suppl_row_newline(this->quartile_range, _SIT::raw);
    s.suppl_row_newline(this->mean, _SIT::raw);
    if (this->standard_deviation == ::features::standard_deviation_err_value) {
        s.suppl_row_newline(_SIT::empty());
    } else {
        s.suppl_row_newline(this->standard_deviation, _SIT::raw);
    }
}

template<typename T>
struct sample {
    _STD string name{};
    _STD vector<T> values{};
    std::size_t empty{0};

    void sort() {
#ifdef MY_SORT
        ::
#else
        _STD
#endif
        sort(values.begin(), values.end());
    }

    [[nodiscard]] ::features::quantitative get_quantitative() const;

    [[nodiscard]] ::features::stat<T> get_stat() const;

    [[nodiscard]] sample<T> first_differences() const;
};

template<typename T>
::features::quantitative sample<T>::get_quantitative() const {
    return ::features::quantitative{values.size(), this->empty, ::common::count_unique(
            this->values.begin(), this->values.end()
    )
    };
}

template<typename T>
::features::stat<T> sample<T>::get_stat() const {//values must be sorted when calling
    if constexpr (::non_categorical<T>) {
        switch (this->values.size()) {
            case 0:
                return ::features::stat<T>{};
            case 1:
                return ::features::stat<T>{values[0], values[0], values[0], values[0],
                                           (double) values[0], ::features::standard_deviation_err_value};
            default: {//vector values must be already sorted
                auto median = ::common::median(this->values.begin(), this->values.end());
                T quartile_range = *::common::median(_STD next(median), this->values.end()) -
                                   *::common::median(this->values.begin(), _STD next(median));
                double mean = ::common::mean(this->values.begin(), this->values.end());
                return ::features::stat<T>{this->values.front(), this->values.back(),
                                           *median, quartile_range, mean,
                                           ::common::standard_deviation(
                                                   this->values.begin(), this->values.end(), mean)};
            }
        }
    } else {
        return ::features::stat<T>{};
    }
}

template<typename T>
sample<T> sample<T>::first_differences() const {
    if constexpr (!::non_categorical<T>) {
        return ::sample<T>();
    } else {
        if (this->values.size() < 2) {
            return ::sample<T>();
        }

        ::sample<T> diffs;
        diffs.name = this->name + ": first differences";
        diffs.values.reserve(this->values.size() - 1);
        for (std::size_t i = 1; i < this->values.size(); ++i) {
            diffs.values.push_back(this->values[i] - this->values[i - 1]);
        }
        return diffs;
    }
}