#pragma once
#include <iostream>
#include <initializer_list>
#include "tmathvector.h"

template <typename T>
class TMatrix;

template <typename T>
TMatrix<T> operator*(const T& scalar, const TMatrix<T>& mat);

template <typename T>
std::ostream& operator<<(std::ostream& os, const TMatrix<T>& mat);

template <typename T>
class TMatrix : public TMathVector<TMathVector<T>> {
public:
    TMatrix(size_t rows = 0, size_t cols = 0, const T& val = T());
    TMatrix(std::initializer_list<std::initializer_list<T>> list);
    TMatrix(const TMathVector<TMathVector<T>>& other);
    TMatrix(TMathVector<TMathVector<T>>&& other) noexcept;

    TMatrix(const TMatrix& other) = default;
    TMatrix(TMatrix&& other) noexcept = default;
    TMatrix& operator=(const TMatrix& other) = default;
    TMatrix& operator=(TMatrix&& other) noexcept = default;
    virtual ~TMatrix() = default;

    size_t rows() const noexcept;
    size_t cols() const noexcept;

    TMatrix& operator*=(const T& scalar);
    TMatrix operator*(const T& scalar) const;
    TMatrix& operator/=(const T& scalar);
    TMatrix operator/(const T& scalar) const;

    TMathVector<T> operator*(const TMathVector<T>& vec) const;

    TMatrix operator*(const TMatrix& other) const;

    TMatrix transpose() const;

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const TMatrix<U>& mat);
};

template <typename T>
TMatrix<T>::TMatrix(size_t rows, size_t cols, const T& val) : TMathVector<TMathVector<T>>(rows) {
    for (size_t i = 0; i < rows; ++i) {
        (*this)[i] = TMathVector<T>(cols, 0, val);
    }
}

template <typename T>
TMatrix<T>::TMatrix(std::initializer_list<std::initializer_list<T>> list) : TMathVector<TMathVector<T>>(list.size()) {
    size_t i = 0;
    for (const auto& row_list : list) {
        (*this)[i++] = TMathVector<T>(row_list);
    }
}

template <typename T>
TMatrix<T>::TMatrix(const TMathVector<TMathVector<T>>& other) : TMathVector<TMathVector<T>>(other) {}

template <typename T>
TMatrix<T>::TMatrix(TMathVector<TMathVector<T>>&& other) noexcept : TMathVector<TMathVector<T>>(std::move(other)) {}

template <typename T>
size_t TMatrix<T>::rows() const noexcept {
    return this->size();
}

template <typename T>
size_t TMatrix<T>::cols() const noexcept {
    return (this->size() > 0) ? (*this)[0].size() : 0;
}

template <typename T>
TMatrix<T>& TMatrix<T>::operator*=(const T& scalar) {
    for (size_t i = 0; i < rows(); i++) {
        (*this)[i] *= scalar;
    }
    return *this;
}

template <typename T>
TMatrix<T> TMatrix<T>::operator*(const T& scalar) const {
    TMatrix res = *this;
    res *= scalar;
    return res;
}

template <typename T>
TMatrix<T>& TMatrix<T>::operator/=(const T& scalar) {
    if (scalar == T(0))
        throw "Division by zero";
    for (size_t i = 0; i < rows(); i++) {
        (*this)[i] /= scalar;
    }
    return *this;
}

template <typename T>
TMatrix<T> TMatrix<T>::operator/(const T& scalar) const {
    TMatrix res = *this;
    res /= scalar;
    return res;
}

// multiply matrix (M x K) and vector (K) -> vector (M)
template <typename T>
TMathVector<T> TMatrix<T>::operator*(const TMathVector<T>& vec) const {
    if (cols() != vec.size())
        throw "Matrix and vector dimensions mismatch for multiplication";
    TMathVector<T> res(rows());
    for (size_t i = 0; i < rows(); i++) {
        res[i] = (*this)[i] * vec;
    }
    return res;
}

// multiply matrix (M x K) * (K x N) -> (M x N)
template <typename T>
TMatrix<T> TMatrix<T>::operator*(const TMatrix& other) const {
    if (cols() != other.rows())
        throw "Matrix dimensions mismatch for multiplication";
    size_t M = rows();
    size_t K = cols();
    size_t N = other.cols();
    TMatrix res(M, N, T(0));
    for (size_t i = 0; i < M; i++) {
        for (size_t k = 0; k < K; k++) {
            for (size_t j = 0; j < N; j++) {
                res[i][j] += (*this)[i][k] * other[k][j];
            }
        }
    }
    return res;
}

// transpose matrix (M x N) -> (N x M)
template <typename T>
TMatrix<T> TMatrix<T>::transpose() const {
    TMatrix res(cols(), rows());
    for (size_t i = 0; i < rows(); ++i) {
        for (size_t j = 0; j < cols(); ++j) {
            res[j][i] = (*this)[i][j];
        }
    }
    return res;
}

template <typename T>
TMatrix<T> operator*(const T& scalar, const TMatrix<T>& mat) {
    return mat * scalar;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const TMatrix<T>& mat) {
    for (size_t i = 0; i < mat.rows(); ++i) {
        os << "[ ";
        for (size_t j = 0; j < mat.cols(); ++j) {
            os << mat[i][j];
            if (j != mat.cols() - 1) os << " ";
        }
        os << " ]";
        if (i != mat.rows() - 1) os << "\n";
    }
    return os;
}
