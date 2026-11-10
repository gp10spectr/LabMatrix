#pragma once
#include <iostream>
#include <initializer_list>
#include "tmatrix.h"

template <typename T>
class TTriangleMatrix;

template <typename T>
TTriangleMatrix<T> operator*(const T& scalar, const TTriangleMatrix<T>& mat);

template <typename T>
std::ostream& operator<<(std::ostream& os, const TTriangleMatrix<T>& mat);

template <typename T>
class TTriangleMatrix : public TMatrix<T> {
public:
    TTriangleMatrix(size_t dim = 0, const T& val = T());
    TTriangleMatrix(std::initializer_list<std::initializer_list<T>> list);

    TTriangleMatrix(const TTriangleMatrix& other) = default;
    TTriangleMatrix(TTriangleMatrix&& other) noexcept = default;
    TTriangleMatrix& operator=(const TTriangleMatrix& other) = default;
    TTriangleMatrix& operator=(TTriangleMatrix&& other) noexcept = default;
    virtual ~TTriangleMatrix() = default;

    size_t raw_row_size(size_t i) const;

    bool operator==(const TTriangleMatrix& other) const;
    bool operator!=(const TTriangleMatrix& other) const;

    TTriangleMatrix operator+(const TTriangleMatrix& other) const;
    TTriangleMatrix operator-(const TTriangleMatrix& other) const;

    TTriangleMatrix& operator*=(const T& scalar);
    TTriangleMatrix operator*(const T& scalar) const;

    TMathVector<T> operator*(const TMathVector<T>& vec) const;

    TTriangleMatrix operator*(const TTriangleMatrix& other) const;

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const TTriangleMatrix<U>& mat);
};

template <typename T>
TTriangleMatrix<T>::TTriangleMatrix(size_t dim, const T& val) : TMatrix<T>(dim) {
    for (size_t i = 0; i < dim; i++) {
        // creates vector with(dim - i) and start_index = i
        (*this)[i] = TMathVector<T>(dim - i, i, val);
    }
}

template <typename T>
TTriangleMatrix<T>::TTriangleMatrix(std::initializer_list<std::initializer_list<T>> list) : TMatrix<T>(list.size()) {
    size_t dim = list.size();
    size_t i = 0;
    for (const auto& row_list : list) {
        if (row_list.size() != dim)
            throw "Initializer list rows must match dimension";

        TMathVector<T> row(dim - i, i);
        size_t j = 0;
        for (const auto& val : row_list) {
            if (j >= i) {
                row[j] = val;
            }
            ++j;
        }
        (*this)[i] = std::move(row);
        ++i;
    }
}

template <typename T>
size_t TTriangleMatrix<T>::raw_row_size(size_t i) const {
    return (*this)[i].size();
}

template <typename T>
bool TTriangleMatrix<T>::operator==(const TTriangleMatrix& other) const {
    if (this->rows() != other.rows())
        return false;
    for (size_t i = 0; i < this->rows(); i++) {
        if ((*this)[i] != other[i])
            return false;
    }
    return true;
}

template <typename T>
bool TTriangleMatrix<T>::operator!=(const TTriangleMatrix& other) const {
    return !(*this == other);
}

template <typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator+(const TTriangleMatrix& other) const {
    if (this->rows() != other.rows())
        throw "Matrix dimensions must match for addition";
    size_t dim = this->rows();
    TTriangleMatrix res(dim);
    for (size_t i = 0; i < dim; i++) {
        for (size_t j = i; j < dim; j++) {
            res[i][j] = (*this)[i][j] + other[i][j];
        }
    }
    return res;
}

template <typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator-(const TTriangleMatrix& other) const {
    if (this->rows() != other.rows())
        throw "Matrix dimensions must match for subtraction";
    size_t dim = this->rows();
    TTriangleMatrix res(dim);
    for (size_t i = 0; i < dim; i++) {
        for (size_t j = i; j < dim; j++) {
            res[i][j] = (*this)[i][j] - other[i][j];
        }
    }
    return res;
}

template <typename T>
TTriangleMatrix<T>& TTriangleMatrix<T>::operator*=(const T& scalar) {
    size_t dim = this->rows();
    for (size_t i = 0; i < dim; i++) {
        for (size_t j = i; j < dim; j++) {
            (*this)[i][j] *= scalar;
        }
    }
    return *this;
}

template <typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator*(const T& scalar) const {
    TTriangleMatrix res = *this;
    res *= scalar;
    return res;
}

template <typename T>
TMathVector<T> TTriangleMatrix<T>::operator*(const TMathVector<T>& vec) const {
    if (this->rows() != vec.size())
        throw "Matrix and vector dimensions mismatch";
    size_t dim = this->rows();
    TMathVector<T> res(dim, 0, T(0));
    for (size_t i = 0; i < dim; i++) {
        for (size_t j = i; j < dim; j++) {
            res[i] += (*this)[i][j] * vec[j];
        }
    }
    return res;
}

template <typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator*(const TTriangleMatrix& other) const {
    if (this->rows() != other.rows())
        throw "Matrix dimensions must match for multiplication";
    size_t dim = this->rows();
    TTriangleMatrix res(dim, T(0));
    for (size_t i = 0; i < dim; i++) {
        for (size_t j = i; j < dim; j++) {
            T sum = T(0);
            for (size_t k = i; k <= j; ++k) {
                sum += (*this)[i][k] * other[k][j];
            }
            res[i][j] = sum;
        }
    }
    return res;
}

template <typename T>
TTriangleMatrix<T> operator*(const T& scalar, const TTriangleMatrix<T>& mat) {
    return mat * scalar;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const TTriangleMatrix<T>& mat) {
    for (size_t i = 0; i < mat.rows(); i++) {
        os << "[ ";
        for (size_t j = 0; j < mat.cols(); j++) {
            os << mat[i][j];
            if (j != mat.cols() - 1) os << " ";
        }
        os << " ]";
        if (i != mat.rows() - 1) os << "\n";
    }
    return os;
}