#pragma once
#include <cstddef>
#include <initializer_list>
#include <algorithm>

#define MEM_STEP 15

inline size_t calculate_capacity(size_t size) {
    if (size == 0) return 0;
    return ((size - 1) / MEM_STEP + 1) * MEM_STEP;
}

template<typename T>
class TVector;

template<typename T>
class MemData {
    T* _data;
    size_t _size;
    size_t _capacity;

public:
    explicit MemData(size_t size = 0);
    MemData(std::initializer_list<T> list);
    MemData(const T* arr, size_t size);
    MemData(const MemData& other);
    MemData(MemData&& other) noexcept;
    ~MemData();

    bool is_empty() const noexcept { return _size == 0; }
    bool is_full() const noexcept { return _size == _capacity; }
    size_t size() const noexcept { return _size; }
    size_t capacity() const noexcept { return _capacity; }
    const T* data() const noexcept { return _data; }

    void set_memory(size_t new_capacity);
    void reset_memory(size_t new_capacity, size_t start_index = 0);
    void clear_memory() noexcept;

    MemData& operator=(const MemData& other);
    MemData& operator=(MemData&& other) noexcept;

    friend class TVector<T>;
};

template<typename T>
MemData<T>::MemData(size_t size) : _data(nullptr), _size(0), _capacity(0) {
    if (size > 0) {
        _capacity = calculate_capacity(size);
        _size = size;
        _data = new T[_capacity]();
    }
}

template<typename T>
MemData<T>::MemData(std::initializer_list<T> list)
    : _data(nullptr), _size(list.size()), _capacity(0) {

    if (_size > 0) {
        _capacity = calculate_capacity(_size);
        _data = new T[_capacity]();
        size_t i = 0;
        for (const T& val : list) {
            _data[i++] = val;
        }
    }
}

template<typename T>
MemData<T>::MemData(const T* arr, size_t size)
    : _data(nullptr), _size(0), _capacity(0) {

    if (size > 0 && arr) {
        _size = size;
        _capacity = calculate_capacity(size);
        _data = new T[_capacity]();
        for (size_t i = 0; i < size; i++) {
            _data[i] = arr[i];
        }
    }
}

template<typename T>
MemData<T>::MemData(const MemData& other)
    : _data(nullptr), _size(other._size), _capacity(other._capacity) {

    if (_capacity > 0) {
        _data = new T[_capacity]();
        for (size_t i = 0; i < _size; i++) {
            _data[i] = other._data[i];
        }
    }
}

template<typename T>
MemData<T>::MemData(MemData&& other) noexcept
    : _data(other._data), _size(other._size), _capacity(other._capacity) {

    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
}

template<typename T>
MemData<T>::~MemData() {
    delete[] _data;
}

template<typename T>
void MemData<T>::set_memory(size_t new_capacity) {
    T* new_data = new_capacity ? new T[new_capacity]() : nullptr;

    size_t copy_size = std::min(_size, new_capacity);
    for (size_t i = 0; i < copy_size; i++) {
        new_data[i] = _data[i];
    }

    delete[] _data;
    _data = new_data;
    _capacity = new_capacity;
    _size = copy_size;
}

template<typename T>
void MemData<T>::reset_memory(size_t new_capacity, size_t start_index) {
    T* new_data = new_capacity ? new T[new_capacity]() : nullptr;

    size_t copy_size = std::min(_size, new_capacity);
    for (size_t i = 0; i < copy_size; i++) {
        size_t dest = start_index + i;
        if (dest < new_capacity) {
            new_data[dest] = _data[i];
        }
    }

    delete[] _data;
    _data = new_data;
    _capacity = new_capacity;
    _size = copy_size;
}

template<typename T>
void MemData<T>::clear_memory() noexcept {
    delete[] _data;
    _data = nullptr;
    _size = 0;
    _capacity = 0;
}

template<typename T>
MemData<T>& MemData<T>::operator=(const MemData& other) {
    if (this != &other) {
        T* new_data = other._capacity ? new T[other._capacity]() : nullptr;
        for (size_t i = 0; i < other._size; i++) {
            new_data[i] = other._data[i];
        }

        delete[] _data;
        _data = new_data;
        _size = other._size;
        _capacity = other._capacity;
    }
    return *this;
}

template<typename T>
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