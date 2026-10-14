#pragma once
#include "vector.h"
#include "tmathvector.h"
#include <initializer_list>
#include <iostream>

template <typename T>
class TMatrix : public TMathVector<TMathVector<T>> {
public:
    TMatrix() = default;
    TMatrix(size_t rows, size_t cols, const T& value = T());
    TMatrix(std::initializer_list<std::initializer_list<T>> list);
    size_t rows() const;
    size_t cols() const;
    TMatrix transpose() const;
    T trace() const;
    T determinant() const;

    TMatrix operator*(const TMatrix& other) const;
    TMatrix& operator*=(const TMatrix& other);
};
