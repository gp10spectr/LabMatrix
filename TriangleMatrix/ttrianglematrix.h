#pragma once
#include "tmatrix.h"
#include <initializer_list>
#include <iostream>

template <typename T>
class TTriangleMatrix {
    TMathVector<TMathVector<T>> _rows;
    size_t _n = 0;
    bool _upper = true;

public:
    class TRowProxy {
        friend class TTriangleMatrix;
        TMathVector<T>* _row;
        size_t _i;
        size_t _n;
        bool _upper;

        TRowProxy(TMathVector<T>* row, size_t i, size_t n, bool upper) : _row(row), _i(i), _n(n), _upper(upper) {}

    public:
        T& operator[](size_t j);
        const T& operator[](size_t j) const;
    };

    size_t row_length(size_t i) const;
    TTriangleMatrix() = default;
    TTriangleMatrix(size_t n, const T& value = T(), bool upper = true);
    TTriangleMatrix(std::initializer_list<std::initializer_list<T>> list, bool upper = true);
    size_t size() const;
    bool is_upper() const;

    TRowProxy operator[](size_t i);
    TRowProxy operator[](size_t i) const;

    template <typename U>
    friend std::ostream& operator<<(std::ostream&, const TTriangleMatrix<U>&);
};

template <typename T>
typename TTriangleMatrix<T>::TRowProxy
TTriangleMatrix<T>::operator[](size_t i) const {
    return TRowProxy(const_cast<TMathVector<T>*>(&_rows[i]), i, _n, _upper);
}

template <typename T>
std::ostream& operator<<(std::ostream&, const TTriangleMatrix<T>&);

template <typename T>
size_t TTriangleMatrix<T>::row_length(size_t i) const {
    return _upper ? (_n - i) : (i + 1);
}

template <typename T>
T& TTriangleMatrix<T>::TRowProxy::operator[](size_t j) {
    if (_upper) {
        if (j < _i) {
            static T empty = T();
            empty = T();
            return empty;
        }
        return (*_row)[j - _i];
    }
    else {
        if (j > _i) {
            static T empty = T();
            empty = T();
            return empty;
        }
        return (*_row)[j];
    }
}

template <typename T>
TTriangleMatrix<T>::TTriangleMatrix(size_t n, const T& value, bool upper)
    : _n(n), _upper(upper), _rows()
{
    for (size_t i = 0; i < n; i++) {
        size_t len = upper ? (n - i) : (i + 1);
        TMathVector<T> row;
        for (size_t k = 0; k < len; k++)
            row.push_back(value);
        _rows.push_back(row);
    }
}

template <typename T>
TTriangleMatrix<T>::TTriangleMatrix(std::initializer_list<std::initializer_list<T>> list, bool upper) : _upper(upper) {
    _n = list.size();

    for (const auto& row_data : list) {
        if (row_data.size() != _n)
            throw "not a square matrix";
    }

    size_t i = 0;
    for (const auto& row_data : list) {
        TMathVector<T> row;
        if (upper) {
            auto it = row_data.begin();
            for (size_t k = 0; k < i; ++k)
                it++;
            for (size_t j = i; j < _n; j++, it++)
                row.push_back(*it);
        }
        else {
            auto it = row_data.begin();
            for (size_t j = 0; j <= i; j++, it++)
                row.push_back(*it);
        }
        if (upper) {
            auto check = row_data.begin();
            for (size_t j = 0; j < i; ++j, check++)
                if (*check != T())
                    throw "nonzero below diagonal in upper triangle";
        }
        else {
            auto check = row_data.begin();
            for (size_t j = 0; j < _n; ++j, check++) {
                if (j <= i)
                    continue;
                if (*check != T())
                    throw "nonzero above diagonal in lower triangle";
            }
        }

        _rows.push_back(row);
        ++i;
    }
}

template <typename T>
size_t TTriangleMatrix<T>::size() const {
    return _n;
}

template <typename T>
bool TTriangleMatrix<T>::is_upper() const {
    return _upper;
}

template <typename T>
typename TTriangleMatrix<T>::TRowProxy TTriangleMatrix<T>::operator[](size_t i) {
    return TRowProxy(&_rows[i], i, _n, _upper);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const TTriangleMatrix<T>& m) {
    for (size_t i = 0; i < m.size(); i++) {
        os << "[ ";
        for (size_t j = 0; j < m.size(); j++) {
            os << m[i][j];
            if (j != m.size() - 1) os << " ";
        }
        os << " ]";
        if (i != m.size() - 1) os << "\n";
    }
    return os;
}