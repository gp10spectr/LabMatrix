#pragma once
#include <cmath>
#include <iostream>
#include "vector.h"

template <typename T>
class TMathVector;

template <typename T>
TMathVector<T> operator*(const T& scalar, const TMathVector<T>& vec);

template <typename T>
std::ostream& operator<<(std::ostream& os, const TMathVector<T>& vec);

template <typename T>
std::istream& operator>>(std::istream& is, TMathVector<T>& vec);

template <typename T>
class TMathVector : protected TVector<T> {
private:
    size_t _start_index;
public:
    using TVector<T>::size;
    using TVector<T>::capacity;
    using TVector<T>::is_empty;

    size_t start_index() const noexcept;

    TMathVector(size_t size = 0, size_t start_index = 0, const T& val = T());
    TMathVector(std::initializer_list<T> list, size_t start_index = 0);
    TMathVector(const T* arr, size_t size, size_t start_index = 0);
    TMathVector(const TVector<T>& other);
    TMathVector(TVector<T>&& other) noexcept;

    TMathVector(const TMathVector& other) = default;
    TMathVector(TMathVector&& other) noexcept = default;
    TMathVector& operator=(const TMathVector& other) = default;
    TMathVector& operator=(TMathVector&& other) noexcept = default;
    virtual ~TMathVector() = default;

    T& operator[](size_t j);
    const T& operator[](size_t j) const;

    bool operator==(const TMathVector& other) const;
    bool operator!=(const TMathVector& other) const;

    TMathVector operator-() const;

    TMathVector& operator+=(const TMathVector& other);
    TMathVector operator+(const TMathVector& other) const;

    TMathVector& operator-=(const TMathVector& other);
    TMathVector operator-(const TMathVector& other) const;

    TMathVector& operator*=(const T& scalar);
    TMathVector operator*(const T& scalar) const;

    TMathVector& operator/=(const T& scalar);
    TMathVector operator/(const T& scalar) const;

    T operator*(const TMathVector& other) const;

    double length() const;

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const TMathVector<U>& vec);

    template <typename U>
    friend std::istream& operator>>(std::istream& is, TMathVector<U>& vec);

    template <typename U>
    friend class TMatrix;

    friend TMathVector operator*(const T& scalar, const TMathVector& vec) {
        return vec * scalar;
    }

    friend std::ostream& operator<<(std::ostream& os, const TMathVector& vec) {
        os << "(";
        for (size_t i = 0; i < vec.size(); ++i) {
            os << vec[vec.start_index() + i];
            if (i != vec.size() - 1) os << ", ";
        }
        os << ")";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, TMathVector& vec) {
        for (size_t i = 0; i < vec.size(); ++i) {
            is >> vec[vec.start_index() + i];
        }
        return is;
    }
};

template <typename T>
size_t TMathVector<T>::start_index() const noexcept {
    return _start_index;
}

template <typename T>
TMathVector<T>::TMathVector(size_t size, size_t start_index, const T& val) : TVector<T>(size), _start_index(start_index) {
    for (size_t i = 0; i < size; i++) {
        TVector<T>::operator[](i) = val;
    }
}

template <typename T>
TMathVector<T>::TMathVector(std::initializer_list<T> list, size_t start_index) : TVector<T>(list), _start_index(start_index) {}

template <typename T>
TMathVector<T>::TMathVector(const T* arr, size_t size, size_t start_index) : TVector<T>(const_cast<T*>(arr), size), _start_index(start_index) {}

template <typename T>
TMathVector<T>::TMathVector(const TVector<T>& other) : TVector<T>(other), _start_index(0) {}

template <typename T>
TMathVector<T>::TMathVector(TVector<T>&& other) noexcept : TVector<T>(std::move(other)), _start_index(0) {}

template <typename T>
T& TMathVector<T>::operator[](size_t j) {
    static T empty = T(0);
    empty = T(0);
    if (j < _start_index) {
        //under diagonal return 0
        return empty;
    }
    if (j - _start_index >= this->size()) {
        throw "Index out of range";
    }
    return TVector<T>::operator[](j - _start_index);
}

template <typename T>
const T& TMathVector<T>::operator[](size_t j) const {
    static const T zero = T(0);
    if (j < _start_index) {
        return zero;
    }
    if (j - _start_index >= this->size()) {
        throw "Index out of range";
    }
    return TVector<T>::operator[](j - _start_index);
}

template <typename T>
bool TMathVector<T>::operator==(const TMathVector& other) const {
    if (this->size() != other.size() || this->_start_index != other._start_index)
        return false;
    for (size_t i = 0; i < this->size(); i++) {
        if (TVector<T>::operator[](i) != other.TVector<T>::operator[](i))
            return false;
    }
    return true;
}

template <typename T>
bool TMathVector<T>::operator!=(const TMathVector& other) const {
    return !(*this == other);
}

template <typename T>
TMathVector<T> TMathVector<T>::operator-() const {
    TMathVector res(this->size(), this->_start_index);
    for (size_t i = 0; i < this->size(); i++) {
        res.TVector<T>::operator[](i) = -TVector<T>::operator[](i);
    }
    return res;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator+=(const TMathVector& other) {
    if (this->size() != other.size() || this->_start_index != other._start_index)
        throw "Vector sizes and start indices must match for addition";
    for (size_t i = 0; i < this->size(); i++) {
        TVector<T>::operator[](i) += other.TVector<T>::operator[](i);
    }
    return *this;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator+(const TMathVector& other) const {
    TMathVector res = *this;
    res += other;
    return res;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator-=(const TMathVector& other) {
    if (this->size() != other.size() || this->_start_index != other._start_index)
        throw "Vector sizes and start indices must match for subtraction";
    for (size_t i = 0; i < this->size(); i++) {
        TVector<T>::operator[](i) -= other.TVector<T>::operator[](i);
    }
    return *this;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator-(const TMathVector& other) const {
    TMathVector res = *this;
    res -= other;
    return res;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator*=(const T& scalar) {
    for (size_t i = 0; i < this->size(); i++) {
        TVector<T>::operator[](i) *= scalar;
    }
    return *this;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator*(const T& scalar) const {
    TMathVector res = *this;
    res *= scalar;
    return res;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator/=(const T& scalar) {
    if (scalar == T(0))
        throw "Division by zero";
    for (size_t i = 0; i < this->size(); i++) {
        TVector<T>::operator[](i) /= scalar;
    }
    return *this;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator/(const T& scalar) const {
    TMathVector res = *this;
    res /= scalar;
    return res;
}

template <typename T>
T TMathVector<T>::operator*(const TMathVector& other) const {
    if (this->size() != other.size() || this->_start_index != other._start_index)
        throw "Vector sizes and start indices must match for dot product";
    T dot_product = T(0);
    for (size_t i = 0; i < this->size(); i++) {
        dot_product += TVector<T>::operator[](i) * other.TVector<T>::operator[](i);
    }
    return dot_product;
}

template <typename T>
double TMathVector<T>::length() const {
    double sum = 0.0;
    for (size_t i = 0; i < this->size(); i++) {
        T val = TVector<T>::operator[](i);
        sum += static_cast<double>(val * val);
    }
    return std::sqrt(sum);
}

template <typename T>
TMathVector<T> operator*(const T& scalar, const TMathVector<T>& vec) {
    return vec * scalar;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const TMathVector<T>& vec) {
    os << "(";
    for (size_t i = 0; i < vec.size(); i++) {
        os << vec[vec.start_index() + i];
        if (i != vec.size() - 1) os << ", ";
    }
    os << ")";
    return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, TMathVector<T>& vec) {
    for (size_t i = 0; i < vec.size(); i++) {
        is >> vec[vec.start_index() + i];
    }
    return is;
}