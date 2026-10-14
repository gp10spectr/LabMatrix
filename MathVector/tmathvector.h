#pragma once
#include "vector.h"
#include <cmath>

template <typename T>
class TMathVector : public TVector<T> {
public:
    //using?
    TMathVector() = default;
    TMathVector(size_t size) : TVector<T>(size) {}
    TMathVector(std::initializer_list<T> list) : TVector<T>(list) {}
    TMathVector(T* arr, size_t size) : TVector<T>(arr, size) {}
    TMathVector(const TVector<T>& other) : TVector<T>(other) {}
    TMathVector(TVector<T>&& other) : TVector<T>(std::move(other)) {}
    TMathVector& operator+=(const TMathVector& other);
    TMathVector& operator-=(const TMathVector& other);

    template <typename Scalar>
    TMathVector& operator*=(const Scalar& scalar);

    template <typename Scalar>
    TMathVector& operator/=(const Scalar& scalar);

    T dot(const TMathVector& other) const;
    double norm() const;
};

template <typename T>
TMathVector<T>& TMathVector<T>::operator+=(const TMathVector& other) {
    if ((*this).size() != other.size())
        throw "vectors have different sizes";
    for (size_t i = 0; i < (*this).size(); i++)
        (*this)[i] += other[i];
    return *this;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator-=(const TMathVector& other) {
    if ((*this).size() != other.size())
        throw "vectors have different sizes";
    for (size_t i = 0; i < (*this).size(); i++)
        (*this)[i] -= other[i];
    return *this;
}

template <typename T>
template <typename Scalar>
TMathVector<T>& TMathVector<T>::operator*=(const Scalar& scalar) {
    for (size_t i = 0; i < (*this).size(); i++)
        (*this)[i] *= scalar;
    return *this;
}

template <typename T>
template <typename Scalar>
TMathVector<T>& TMathVector<T>::operator/=(const Scalar& scalar) {
    for (size_t i = 0; i < (*this).size(); i++)
        (*this)[i] /= scalar;
    return *this;
}

template <typename T>
T TMathVector<T>::dot(const TMathVector& other) const {
    if ((*this).size() != other.size())
        throw "size are not equal";
    T result = T();
    for (size_t i = 0; i < (*this).size(); i++)
        result += (*this)[i] * other[i];
    return result;
}

template <typename T>
double TMathVector<T>::norm() const {
    double sum = 0.0;
    for (size_t i = 0; i < (*this).size(); i++) {
        double v = static_cast<double>((*this)[i]);
        sum += v * v;
    }
    return std::sqrt(sum);
}

template <typename T>
TMathVector<T> operator+(TMathVector<T> lhs, const TMathVector<T>& rhs) {
    lhs += rhs;
    return lhs;
}

template <typename T>
TMathVector<T> operator-(TMathVector<T> lhs, const TMathVector<T>& rhs) {
    lhs -= rhs;
    return lhs;
}

template <typename T, typename Scalar>
TMathVector<T> operator*(const TMathVector<T>& v, const Scalar& scalar) {
    TMathVector<T> result(v);
    result *= scalar;
    return result;
}

template <typename T, typename Scalar>
TMathVector<T> operator*(const Scalar& scalar, const TMathVector<T>& v) {
    return v * scalar;
}

template <typename T, typename Scalar>
TMathVector<T> operator/(const TMathVector<T>& v, const Scalar& scalar) {
    TMathVector<T> result(v);
    result /= scalar;
    return result;
}
