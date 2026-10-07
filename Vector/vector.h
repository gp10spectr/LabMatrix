#pragma once
#include <iostream>
#include "memdata.h"

template <typename T>
class TVector;

template <typename T>
std::ostream& operator<<(std::ostream&, const TVector<T>&);
template <typename T>
std::istream& operator>>(std::istream&, TVector<T>&);

template <typename T>
class TVector {
    MemData<T> _mem;
    size_t _front;
    size_t _back;

    void linearize();

public:
    TVector(size_t size = 0);
    TVector(std::initializer_list<T>);
    TVector(T*, size_t);
    TVector(const TVector&);
    TVector(TVector&&);
    ~TVector() = default;

    inline bool is_empty() const noexcept { return _mem.is_empty(); }
    inline bool is_full() const noexcept { return _mem.is_full(); }

    inline size_t size() const noexcept { return _mem.size(); }
    inline size_t capacity() const noexcept { return _mem.capacity(); }

    inline T front() const;
    inline T back() const;
    inline T& front();
    inline T& back();

    void push_front(const T&);
    void push_back(const T&);
    void insert(const T&, size_t);
    void pop_front();
    void pop_back();
    void erase(size_t);

    void push_front(size_t n, const T& value);
    void push_back(size_t n, const T& value);
    void insert(const T& value, size_t pos, size_t n);
    void pop_front(size_t n);
    void pop_back(size_t n);
    void erase(size_t pos, size_t n);

    TVector& operator=(const TVector&) noexcept;
    TVector& operator=(TVector&&) noexcept;

    T operator[](size_t) const noexcept;
    T& operator[](size_t) noexcept;

    template <typename U>
    friend std::ostream& operator<<(std::ostream&, const TVector<U>&);
    template <typename U>
    friend std::istream& operator>>(std::istream&, TVector<U>&);
};

template <typename T>
void TVector<T>::linearize() {
    if (_mem._capacity == 0 || _mem._size == 0 || _front == 0)
        return;
    T* temp = new T[_mem._size];
    for (size_t i = 0; i < _mem._size; ++i)
        temp[i] = _mem._data[(_front + i) % _mem._capacity];
    for (size_t i = 0; i < _mem._size; ++i)
        _mem._data[i] = temp[i];
    delete[] temp;
    _front = 0;
    _back = _mem._size - 1;
}

template <typename T>
TVector<T>::TVector(size_t size)
    : _mem(size), _front(0), _back(size ? size - 1 : 0) {}

template <typename T>
TVector<T>::TVector(std::initializer_list<T> list)
    : _mem(list), _front(0), _back(list.size() ? list.size() - 1 : 0) {}

template <typename T>
TVector<T>::TVector(T* arr, size_t size)
    : _mem(arr, size), _front(0), _back(size ? size - 1 : 0) {}

template <typename T>
TVector<T>::TVector(const TVector& other)
    : _mem(other._mem), _front(other._front), _back(other._back) {}

template <typename T>
TVector<T>::TVector(TVector&& other)
    : _mem(std::move(other._mem)), _front(other._front), _back(other._back) {
    other._front = other._back = 0;
}

template <typename T>
T TVector<T>::operator[](size_t idx) const noexcept {
    size_t cap = _mem._capacity;
    if (cap == 0)
        return T();
    idx = (_front + idx) % cap;
    return _mem._data[idx];
}

template <typename T>
T& TVector<T>::operator[](size_t idx) noexcept {
    size_t cap = _mem._capacity;
    if (cap != 0) 
        idx = (_front + idx) % cap;
    return _mem._data[idx];
}

template <typename T>
T TVector<T>::front() const {
    if (is_empty())
        throw "Vector is empty";
    return _mem._data[_front];
}

template <typename T>
T TVector<T>::back() const {
    if (is_empty())
        throw "Vector is empty";
    return _mem._data[_back];
}

template <typename T>
T& TVector<T>::front() {
    if (is_empty())
        throw "Vector is empty";
    return _mem._data[_front];
}

template <typename T>
T& TVector<T>::back() {
    if (is_empty())
        throw "Vector is empty";
    return _mem._data[_back];
}

template <typename T>
void TVector<T>::push_front(const T& value) {
    if (is_empty()) {
        _mem.reset_memory(1, 0);
        _mem._data[0] = value;
        _front = _back = 0;
        return;
    }

    if (is_full()) {
        linearize();
        size_t old_size = _mem._size;
        _mem.reset_memory(old_size + 1, 1);
        _mem._data[0] = value;
        _front = 0;
        _back = old_size;
    }
    else {
        _front = (_front == 0) ? _mem._capacity - 1 : _front - 1;
        _mem._data[_front] = value;
        _mem._size++;
    }
}

template <typename T>
void TVector<T>::push_back(const T& value) {
    if (is_empty()) {
        _mem.reset_memory(1, 0);
        _mem._data[0] = value;
        _front = _back = 0;
        return;
    }

    if (is_full()) {
        linearize();
        size_t old_size = _mem._size;
        _mem.reset_memory(old_size + 1, 0);
        _mem._data[old_size] = value;
        _front = 0;
        _back = old_size;
    }
    else {
        _back = (_back + 1) % _mem._capacity;
        _mem._data[_back] = value;
        _mem._size++;
    }
}

template <typename T>
void TVector<T>::insert(const T& value, size_t pos) {
    if (pos > size())
        throw "Invalid position";
    if (pos == 0) {
        push_front(value);
        return;
    }
    if (pos == size()) {
        push_back(value);
        return;
    }

    size_t old_size = _mem._size;
    size_t cap = _mem._capacity;

    if (old_size < cap) {
        for (size_t i = old_size; i > pos; --i) {
            size_t src = (_front + i - 1) % cap;
            size_t dst = (_front + i) % cap;
            _mem._data[dst] = _mem._data[src];
        }
        _mem._data[(_front + pos) % cap] = value;
        _mem._size++;
        _back = (_front + _mem._size - 1) % cap;
    }
    else {
        linearize();
        size_t new_size = old_size + 1;
        _mem.reset_memory(new_size, 0);
        for (size_t i = old_size; i > pos; --i)
            _mem._data[i] = _mem._data[i - 1];
        _mem._data[pos] = value;
        _front = 0;
        _back = new_size - 1;
    }
}

template <typename T>
void TVector<T>::pop_front() {
    if (is_empty())
        throw "Vector is empty";

    if (_mem._size == 1) {
        _mem.clear_memory();
        _front = _back = 0;
        return;
    }

    _front = (_front + 1) % _mem._capacity;
    _mem._size--;

    if (_mem._capacity > MEM_STEP && _mem._size + MEM_STEP <= _mem._capacity) {
        linearize();
        _mem.reset_memory(_mem._size, 0);
        _front = 0;
        _back = _mem._size - 1;
    }
}

template <typename T>
void TVector<T>::pop_back() {
    if (is_empty())
        throw "Vector is empty";

    if (_mem._size == 1) {
        _mem.clear_memory();
        _front = _back = 0;
        return;
    }

    _back = (_back == 0) ? _mem._capacity - 1 : _back - 1;
    _mem._size--;

    if (_mem._capacity > MEM_STEP && _mem._size + MEM_STEP <= _mem._capacity) {
        linearize();
        _mem.reset_memory(_mem._size, 0);
        _front = 0;
        _back = _mem._size - 1;
    }
}

template <typename T>
void TVector<T>::erase(size_t pos) {
    if (pos >= size())
        throw "Invalid position";
    if (pos == 0) {
        pop_front();
        return;
    }
    if (pos == size() - 1) {
        pop_back();
        return;
    }

    size_t cap = _mem._capacity;
    size_t S = _mem._size;
    for (size_t i = pos; i < S - 1; ++i) {
        size_t src = (_front + i + 1) % cap;
        size_t dst = (_front + i) % cap;
        _mem._data[dst] = _mem._data[src];
    }
    pop_back();
}

template <typename T>
void TVector<T>::push_front(size_t n, const T& value) {
    if (n == 0)
        return;
    if (is_empty()) {
        _mem.reset_memory(n, 0);
        for (size_t i = 0; i < n; ++i) 
            _mem._data[i] = value;
        _front = 0;
        _back = n - 1;
        return;
    }

    size_t old_size = _mem._size;
    if (old_size + n <= _mem._capacity) {
        for (size_t i = 0; i < n; ++i) {
            _front = (_front == 0) ? _mem._capacity - 1 : _front - 1;
            _mem._data[_front] = value;
        }
        _mem._size += n;
    }
    else {
        linearize();
        _mem.reset_memory(old_size + n, n);
        for (size_t i = 0; i < n; ++i) 
            _mem._data[i] = value;
        _front = 0;
        _back = old_size + n - 1;
    }
}

template <typename T>
void TVector<T>::push_back(size_t n, const T& value) {
    if (n == 0)
        return;
    if (is_empty()) {
        _mem.reset_memory(n, 0);
        for (size_t i = 0; i < n; ++i) 
            _mem._data[i] = value;
        _front = 0;
        _back = n - 1;
        return;
    }

    size_t old_size = _mem._size;
    if (old_size + n <= _mem._capacity) {
        for (size_t i = 0; i < n; ++i) {
            _back = (_back + 1) % _mem._capacity;
            _mem._data[_back] = value;
        }
        _mem._size += n;
    }
    else {
        linearize();
        _mem.reset_memory(old_size + n, 0);
        for (size_t i = 0; i < n; ++i) 
            _mem._data[old_size + i] = value;
        _front = 0;
        _back = old_size + n - 1;
    }
}

template <typename T>
void TVector<T>::insert(const T& value, size_t pos, size_t n) {
    if (pos > size())
        throw "Invalid position";
    if (n == 0)
        return;

    size_t old_size = _mem._size;
    size_t cap = _mem._capacity;

    if (old_size + n <= cap) {
        for (size_t i = old_size; i > pos; --i) {
            size_t src = (_front + i - 1) % cap;
            size_t dst = (_front + i - 1 + n) % cap;
            _mem._data[dst] = _mem._data[src];
        }
        for (size_t i = 0; i < n; ++i)
            _mem._data[(_front + pos + i) % cap] = value;
        _mem._size += n;
        _back = (_front + _mem._size - 1) % cap;
    }
    else {
        linearize();
        _mem.reset_memory(old_size + n, 0);
        for (size_t i = old_size; i > pos; --i)
            _mem._data[i + n - 1] = _mem._data[i - 1];
        for (size_t i = 0; i < n; ++i)
            _mem._data[pos + i] = value;
        _front = 0;
        _back = old_size + n - 1;
    }
}

template <typename T>
void TVector<T>::pop_front(size_t n) {
    if (n > _mem._size)
        throw "Too many elements";
    if (n == 0)
        return;

    if (n == _mem._size) {
        _mem.clear_memory();
        _front = _back = 0;
        return;
    }

    _front = (_front + n) % _mem._capacity;
    _mem._size -= n;

    if (_mem._capacity > MEM_STEP && _mem._size + MEM_STEP <= _mem._capacity) {
        linearize();
        _mem.reset_memory(_mem._size, 0);
        _front = 0;
        _back = _mem._size - 1;
    }
}

template <typename T>
void TVector<T>::pop_back(size_t n) {
    if (n > _mem._size)
        throw "Too many elements";
    if (n == 0)
        return;

    if (n == _mem._size) {
        _mem.clear_memory();
        _front = _back = 0;
        return;
    }

    _mem._size -= n;
    _back = (_front + _mem._size - 1) % _mem._capacity;

    if (_mem._capacity > MEM_STEP && _mem._size + MEM_STEP <= _mem._capacity) {
        linearize();
        _mem.reset_memory(_mem._size, 0);
        _front = 0;
        _back = _mem._size - 1;
    }
}

template <typename T>
void TVector<T>::erase(size_t pos, size_t n) {
    if (n == 0)
        return;
    if (pos + n > size())
        throw "Invalid position";

    size_t cap = _mem._capacity;
    size_t S = _mem._size;
    for (size_t i = pos; i + n < S; ++i) {
        size_t src = (_front + i + n) % cap;
        size_t dst = (_front + i) % cap;
        _mem._data[dst] = _mem._data[src];
    }

    _mem._size -= n;
    _back = (_front + _mem._size - 1) % cap;

    if (cap > MEM_STEP && _mem._size + MEM_STEP <= cap) {
        linearize();
        _mem.reset_memory(_mem._size, 0);
        _front = 0;
        _back = _mem._size - 1;
    }
}

template <typename T>
TVector<T>& TVector<T>::operator=(const TVector& other) noexcept {
    if (this != &other) {
        _mem = other._mem;
        _front = other._front;
        _back = other._back;
    }
    return *this;
}

template <typename T>
TVector<T>& TVector<T>::operator=(TVector&& other) noexcept {
    if (this != &other) {
        _mem = std::move(other._mem);
        _front = other._front;
        _back = other._back;
        other._front = other._back = 0;
    }
    return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const TVector<T>& vec) {
    os << "{ ";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i != vec.size() - 1) os << ", ";
    }
    os << " }";
    return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, TVector<T>& vec) {
    size_t n;
    is >> n;
    vec._mem.clear_memory();
    vec._front = vec._back = 0;
    for (size_t i = 0; i < n; ++i) {
        T x;
        is >> x;
        vec.push_back(x);
    }
    return is;
}