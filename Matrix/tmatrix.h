#pragma once
#include "tmathvector.h"
#include <initializer_list>
#include <iostream>

template <typename T>
class TMatrix : public TMathVector<TMathVector<T>> {
public:
    //using?
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

template <typename T>
std::ostream& operator<<(std::ostream& os, const TMatrix<T>& m);

template <typename T>
TMatrix<T>::TMatrix(size_t rows, size_t cols, const T& value) {
    for (size_t i = 0; i < rows; i++) {
        TMathVector<T> row;
        row.push_back(cols, value);
        (*this).push_back(row);
    }
}

template <typename T>
TMatrix<T>::TMatrix(std::initializer_list<std::initializer_list<T>> list) {
    if (list.size() == 0)
        return;
    size_t c = (*list.begin()).size();
    for (const auto& row_data : list) {
        if (row_data.size() != c)
            throw "rows have different sizes";
        (*this).push_back(TMathVector<T>(row_data));
    }
}

template <typename T>
size_t TMatrix<T>::rows() const { 
    return (*this).size();
}

template <typename T>
size_t TMatrix<T>::cols() const {
    if ((*this).size() == 0)
        return 0;
    return (*this)[0].size();
}

template <typename T>
TMatrix<T> TMatrix<T>::transpose() const {
    TMatrix<T> result;
    for (size_t j = 0; j < cols(); j++) {
        TMathVector<T> row;
        for (size_t i = 0; i < rows(); i++)
            row.push_back((*this)[i][j]);
        result.push_back(row);
    }
    return result;
}

template <typename T>
T TMatrix<T>::trace() const {
    if (rows() != cols())
        throw "it is not a square matrix";
    T sum = T();
    for (size_t i = 0; i < rows(); i++)
        sum += (*this)[i][i];
    return sum;
}

template <typename T>
T TMatrix<T>::determinant() const {
    if (rows() != cols())
        throw "it is not a square matrix";
    size_t n = rows();
    if (n == 0) 
        return T(1);
    if (n == 1) 
        return (*this)[0][0];
    if (n == 2)
        return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
    T det = T();
    int sign = 1;
    for (size_t j = 0; j < n; j++) {
        TMatrix<T> minor;
        for (size_t i = 1; i < n; i++) {
            TMathVector<T> row;
            for (size_t k = 0; k < n; k++) {
                if (k == j)
                    continue;
                row.push_back((*this)[i][k]);
            }
            minor.push_back(row);
        }
        det += sign * (*this)[0][j] * minor.determinant();
        sign = -sign;
    }
    return det;
}

template <typename T>
TMatrix<T> TMatrix<T>::operator*(const TMatrix& other) const {
    if (cols() != other.rows())
        throw "matrix size issues";
    TMatrix<T> result(rows(), other.cols());
    for (size_t i = 0; i < rows(); i++)
        for (size_t j = 0; j < other.cols(); j++) {
            T sum = T();
            for (size_t k = 0; k < cols(); k++)
                sum += (*this)[i][k] * other[k][j];
            result[i][j] = sum;
        }
    return result;
}

template <typename T>
TMatrix<T>& TMatrix<T>::operator*=(const TMatrix& other) {
    *this = (*this) * other;
    return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const TMatrix<T>& m) {
    for (size_t i = 0; i < m.rows(); i++) {
        os << "[ ";
        for (size_t j = 0; j < m.cols(); j++) {
            os << m[i][j];
            if (j != m.cols() - 1) os << " ";
        }
        os << " ]";
        if (i != m.rows() - 1) os << "\n";
    }
    return os;
}