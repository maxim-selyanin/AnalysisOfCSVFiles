#pragma once
#include <iterator>

#define CORR ::corr_algs::

template<typename T>
int sign(const T &t) {
    return (T(0) < val) - (val < T(0));
}

namespace corr_algs {
    constexpr double corr_err_value = 42.;

    template<typename Iter>
    double cov(Iter b1, Iter e1, Iter b2, Iter e2,
            double mean1, double mean2);

    template<typename Iter>
    double Fechner(Iter b1, Iter e1, Iter b2, Iter e2,
            double mean1, double mean2);

    template<typename Iter>
    double Pearson(Iter b1, Iter e1, Iter b2, Iter e2,
            double mean1, double mean2, double sd1, double sd2);
}

template<typename Iter>
double corr_algs::cov(Iter b1, Iter e1, Iter b2, Iter e2,
        double mean1, double mean2) {
    typename Iter::difference_type size = _STD distance(b1, e1);
    if (size != _STD distance(b2, e2) || size <= 1) {
        return CORR corr_err_value;
    }

    using T = typename Iter::value_type;
    auto term = [mean1, mean2](const T &t1, const T &t2) {
        return (t1 - mean1)*(t2 - mean2);
    };

    double result = term(*b1++, *b2++) + term(*b1++, *b2++);
    _STD size_t n = 2;

    while (b1 != e1) {
        result *= double(n - 1)/n;
        result += term(*b1++, *b2++)/n++;
    }

    return result;
}

template<typename Iter>
double corr_algs::Fechner(Iter b1, Iter e1, Iter b2, Iter e2,
        double mean1, double mean2) {
    typename Iter::difference_type size = _STD distance(b1, e1);
    if (size != _STD distance(b2, e2) || size <= 0) {
        return CORR corr_err_value;
    }

    std::size_t coincidence = 0;
    while (b1 != e1) {
        if (::sign<double>(*b1++ - mean1) == ::sign<double>(*b2++ - mean2)) {
            ++coincidence;
        }
    }
    return 2.0 * coincidence / size - 1;
}

template<typename Iter>
double corr_algs::Pearson(Iter b1, Iter e1, Iter b2, Iter e2,
        double mean1, double mean2, double sd1, double sd2) {
    typename Iter::difference_type size = _STD distance(b1, e1);
    if (size != _STD distance(b2, e2) || size <= 1) {
        return CORR corr_err_value;
    }

    return CORR cov(b1, e1, b2, e2, mean1, mean2) / (sd1*sd2);
}