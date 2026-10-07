#pragma once
#include <initializer_list>
#include <cstddef>

#define MEM_STEP 15

int calculate_capacity(int);

template <typename T>
class TVector;

template <typename T>
class MemData {
    T* _data;
    size_t _size;
    size_t _capacity;

    static void copy_items(const T* src, T* dst, size_t n) {
        for (size_t i = 0; i < n; ++i) dst[i] = src[i];
    }

public:
    MemData(size_t size = 0);
    MemData(std::initializer_list<T>);
    MemData(T*, size_t);
    MemData(const MemData&);
    MemData(MemData&&);
    ~MemData();

    inline bool is_empty() const noexcept { return _size == 0; }
    inline bool is_full() const noexcept { return _size == _capacity; }

    inline size_t size() const noexcept { return _size; }
    inline size_t capacity() const noexcept { return _capacity; }
    inline const T* const data() const noexcept { return _data; }

    void set_memory(size_t);
    void reset_memory(size_t size, size_t start_index = 0);
    void clear_memory() noexcept;

    MemData& operator=(const MemData&);
    MemData& operator=(MemData&&) noexcept;

    template <typename U> friend class TVector;
};

template <typename T>
MemData<T>::MemData(size_t size) : _data(nullptr), _size(size), _capacity(0) {
    if (size > 0) {
        _capacity = calculate_capacity(static_cast<int>(size));
        _data = new T[_capacity]();
    }
}

template <typename T>
MemData<T>::MemData(std::initializer_list<T> list)
    : _data(nullptr), _size(list.size()), _capacity(0)
{
    if (_size > 0) {
        _capacity = calculate_capacity(static_cast<int>(_size));
        _data = new T[_capacity]();
        const T* src = list.begin();
        for (size_t i = 0; i < _size; ++i) _data[i] = src[i];
    }
}

template <typename T>
MemData<T>::MemData(T* arr, size_t size)
    : _data(nullptr), _size(size), _capacity(0)
{
    if (size > 0) {
        _capacity = calculate_capacity(static_cast<int>(size));
        _data = new T[_capacity]();
        for (size_t i = 0; i < size; ++i) _data[i] = arr[i];
    }
}

template <typename T>
MemData<T>::MemData(const MemData& other)
    : _data(nullptr), _size(other._size), _capacity(other._capacity)
{
    if (_capacity > 0) {
        _data = new T[_capacity]();
        copy_items(other._data, _data, _capacity);
    }
}

template <typename T>
MemData<T>::MemData(MemData&& other)
    : _data(other._data), _size(other._size), _capacity(other._capacity)
{
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
}

template <typename T>
MemData<T>::~MemData() { delete[] _data; }

template <typename T>
void MemData<T>::set_memory(size_t new_capacity) {
    if (new_capacity == _capacity) return;
    T* new_data = nullptr;
    if (new_capacity > 0) new_data = new T[new_capacity]();
    delete[] _data;
    _data = new_data;
    _capacity = new_capacity;
    if (_size > _capacity) _size = _capacity;
}

template <typename T>
void MemData<T>::reset_memory(size_t new_size, size_t start_index) {
    size_t new_capacity = calculate_capacity(static_cast<int>(new_size));

    if (new_capacity == _capacity && new_size <= _capacity) {
        if (start_index != 0 && _size > 0) {
            T* temp = new T[_size];
            copy_items(_data, temp, _size);
            for (size_t i = 0; i < _capacity; ++i) _data[i] = T();
            for (size_t i = 0; i < _size && i < new_size; ++i)
                _data[(start_index + i) % _capacity] = temp[i];
            delete[] temp;
        }
        _size = new_size;
        return;
    }

    T* new_data = nullptr;
    if (new_capacity > 0) new_data = new T[new_capacity]();
    size_t copy_count = (_size < new_size) ? _size : new_size;
    for (size_t i = 0; i < copy_count; ++i)
        new_data[(start_index + i) % new_capacity] = _data[i];
    delete[] _data;
    _data = new_data;
    _capacity = new_capacity;
    _size = new_size;
}

template <typename T>
void MemData<T>::clear_memory() noexcept {
    delete[] _data;
    _data = nullptr;
    _size = 0;
    _capacity = 0;
}

template <typename T>
MemData<T>& MemData<T>::operator=(const MemData& other) {
    if (this != &other) {
        delete[] _data;
        _size = other._size;
        _capacity = other._capacity;
        if (_capacity > 0) {
            _data = new T[_capacity]();
            copy_items(other._data, _data, _capacity);
        }
        else {
            _data = nullptr;
        }
    }
    return *this;
}

template <typename T>
MemData<T>& MemData<T>::operator=(MemData&& other) noexcept {
    if (this != &other) {
        delete[] _data;
        _data = other._data;
        _size = other._size;
        _capacity = other._capacity;
        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }
    return *this;
}