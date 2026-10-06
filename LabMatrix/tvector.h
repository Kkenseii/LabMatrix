#pragma once
#include <iostream>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <utility>
#include "memdata.h"

template<typename T>
class TVector {
    MemData<T> _mem;
    mutable size_t _front = 0;
    mutable size_t _back = 0;

    size_t normalize_index(size_t index) const {
        return (_front + index) % _mem._capacity;
    }

    void copy_to_linear(T* dest) const {
        for (size_t i = 0; i < _mem._size; i++) {
            dest[i] = _mem._data[(_front + i) % _mem._capacity];
        }
    }

    void make_contiguous() const {
        if (_front + _mem._size > _mem._capacity) {
            std::rotate(_mem._data, _mem._data + _front, _mem._data + _mem._capacity);
            _front = 0;
            _back = _mem._size - 1;
        }
    }

    void relocate(size_t new_capacity) {
        MemData<T> new_mem;
        new_mem.reset_memory(new_capacity);

        copy_to_linear(new_mem._data);
        new_mem._size = _mem._size;

        _mem = std::move(new_mem);
        _front = 0;
        _back = _mem._size ? _mem._size - 1 : 0;
    }

    void realloc_for_insert() {
        if (!_mem.is_full()) return;

        relocate(calculate_capacity(_mem._capacity + 1));
    }

    void realloc_for_delete() {
        if (_mem._capacity >= _mem._size + 2 * MEM_STEP) {
            relocate(calculate_capacity(_mem._size));
        }
    }

public:
    TVector() = default;

    explicit TVector(size_t size) : _mem(size), _front(0), _back(size ? size - 1 : 0) {}

    TVector(std::initializer_list<T> list)
        : _mem(list), _front(0), _back(list.size() ? list.size() - 1 : 0) {
    }

    TVector(const T* arr, size_t size)
        : _mem(arr, size), _front(0) {
        _back = _mem.size() ? _mem.size() - 1 : 0;
    }

    TVector(const TVector& other) {
        if (!other.is_empty()) {
            MemData<T> m;
            m.reset_memory(other._mem._capacity);
            other.copy_to_linear(m._data);
            m._size = other._mem._size;

            _mem = std::move(m);
            _front = 0;
            _back = _mem._size - 1;
        }
    }

    TVector(TVector&& other) noexcept
        : _mem(std::move(other._mem)), _front(other._front), _back(other._back) {
        other._front = other._back = 0;
    }

    bool is_empty() const noexcept { return _mem._size == 0; }
    bool is_full() const noexcept { return _mem._size == _mem._capacity; }

    size_t size() const noexcept { return _mem._size; }
    size_t capacity() const noexcept { return _mem._capacity; }

    T& front() {
        if (is_empty()) throw std::out_of_range("empty");
        return _mem._data[_front];
    }

    T& back() {
        if (is_empty()) throw std::out_of_range("empty");
        return _mem._data[_back];
    }

    const T& front() const {
        if (is_empty()) throw std::out_of_range("empty");
        return _mem._data[_front];
    }

    const T& back() const {
        if (is_empty()) throw std::out_of_range("empty");
        return _mem._data[_back];
    }

    T& operator[](size_t index) {
        if (index >= _mem._size) throw std::out_of_range("index");
        return _mem._data[normalize_index(index)];
    }

    const T& operator[](size_t index) const {
        if (index >= _mem._size) throw std::out_of_range("index");
        return _mem._data[normalize_index(index)];
    }

    void push_front(const T& value) {
        T copy = value;
        realloc_for_insert();

        if (is_empty()) {
            _front = _back = 0;
        }
        else {
            _front = (_front == 0 ? _mem._capacity - 1 : _front - 1);
        }

        _mem._data[_front] = copy;
        _mem._size++;
    }

    void insert(const T& value, size_t pos) {
        if (pos > _mem._size) throw std::out_of_range("pos");

        if (pos == 0) return push_front(value);
        if (pos == _mem._size) return push_back(value);

        T copy = value;
        realloc_for_insert();

        if (pos < _mem._size / 2) {
            _front = (_front == 0 ? _mem._capacity - 1 : _front - 1);

            for (size_t i = 0; i < pos; i++) {
                size_t src = normalize_index(i + 1);
                size_t dst = normalize_index(i);
                _mem._data[dst] = _mem._data[src];
            }
        }
        else {
            _back = (_back + 1) % _mem._capacity;

            for (size_t i = _mem._size; i > pos; i--) {
                size_t src = normalize_index(i - 1);
                size_t dst = normalize_index(i);
                _mem._data[dst] = _mem._data[src];
            }
        }

        _mem._data[normalize_index(pos)] = copy;
        _mem._size++;
    }

    void pop_back() {
        if (is_empty()) throw std::out_of_range("empty");

        _back = (_back == 0 ? _mem._capacity - 1 : _back - 1);
        _mem._size--;

        if (is_empty()) _front = _back = 0;

        realloc_for_delete();
    }

    void pop_front() {
        if (is_empty()) throw std::out_of_range("empty");

        _front = (_front + 1) % _mem._capacity;
        _mem._size--;

        if (is_empty()) _front = _back = 0;

        realloc_for_delete();
    }

    void erase(size_t pos) {
        if (pos >= _mem._size) throw std::out_of_range("pos");

        if (pos == 0) return pop_front();
        if (pos == _mem._size - 1) return pop_back();

        if (pos < _mem._size / 2) {
            for (size_t i = pos; i > 0; i--) {
                (*this)[i] = (*this)[i - 1];
            }
            pop_front();
        }
        else {
            for (size_t i = pos; i < _mem._size - 1; i++) {
                (*this)[i] = (*this)[i + 1];
            }
            pop_back();
        }
    }

    void clear() {
        _mem.clear_memory();
        _front = _back = 0;
    }

    void shrink_to_fit() {
        if (_mem._capacity == _mem._size) return;
        relocate(_mem._size);
    }

    void sort() {
        if (_mem._size <= 1) return;

        make_contiguous();
        std::sort(_mem._data + _front, _mem._data + _front + _mem._size);
    }

    void shuffle() {
        if (_mem._size < 2) return;

        static std::mt19937 gen(std::random_device{}());

        for (size_t i = _mem._size - 1; i > 0; i--) {
            std::uniform_int_distribution<size_t> d(0, i);
            size_t j = d(gen);
            std::swap((*this)[i], (*this)[j]);
        }
    }

    TVector& operator=(const TVector& other) {
        if (this != &other) {
            TVector tmp(other);
            *this = std::move(tmp);
        }
        return *this;
    }

    TVector& operator=(TVector&& other) noexcept {
        if (this != &other) {
            _mem = std::move(other._mem);
            _front = other._front;
            _back = other._back;
            other._front = other._back = 0;
        }
        return *this;
    }

    void push_back(const T& value) {
        T copy = value;
        realloc_for_insert();

        if (is_empty()) {
            _front = _back = 0;
        }
        else {
            _back = (_back + 1) % _mem._capacity;
        }

        _mem._data[_back] = copy;
        _mem._size++;
    }

    friend std::ostream& operator<<(std::ostream& os, const TVector& v) {
        os << "{ ";
        for (size_t i = 0; i < v.size(); i++) {
            os << v[i];
            if (i + 1 < v.size()) os << ", ";
        }
        os << " }";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, TVector& v) {
        size_t n;
        if (!(is >> n)) return is;

        TVector tmp;
        for (size_t i = 0; i < n; i++) {
            T x;
            if (!(is >> x)) return is;
            tmp.push_back(x);
        }
        v = std::move(tmp);
        return is;
    }

    template <class Type>
    class Iterator {
    private:
        Type* p_cur;

    public:
        Iterator() {
            p_cur = nullptr;
        }
        Iterator(Type* ptr) {
            p_cur = ptr;
        }
        Iterator(const Iterator& other) {
            p_cur = other.p_cur;
        }

        Iterator& operator=(const Iterator& other) noexcept {
            if (this != &other) {
                p_cur = other.p_cur;
            }
            return *this;
        }

        bool operator==(const Iterator& other) const noexcept {
            return p_cur == other.p_cur;
        }
        bool operator!=(const Iterator& other) const noexcept {
            return p_cur != other.p_cur;
        }

        Iterator& operator++() noexcept {
            p_cur++;
            return *this;
        }
        Iterator operator++(int) noexcept {
            Iterator temp = *this;
            p_cur++;
            return temp;
        }

        Iterator& operator--() noexcept {
            p_cur--;
            return *this;
        }
        Iterator operator--(int) noexcept {
            Iterator temp = *this;
            p_cur--;
            return temp;
        }

        Iterator operator+(int n) const noexcept {
            return Iterator(p_cur + n);
        }
        Iterator operator-(int n) const noexcept {
            return Iterator(p_cur - n);
        }
        Iterator& operator+=(int n) noexcept {
            p_cur += n;
            return *this;
        }
        Iterator& operator-=(int n) noexcept {
            p_cur -= n;
            return *this;
        }

        Type& operator*() noexcept {
            return *p_cur;
        }
        Type& operator*() const noexcept {
            return *p_cur;
        }
    };

    typedef Iterator<T> iterator;
    typedef Iterator<const T> const_iterator;

    iterator begin() noexcept {
        make_contiguous();
        return iterator(_mem._data + _front);
    }
    iterator end() noexcept {
        make_contiguous();
        return iterator(_mem._data + _front + _mem._size);
    }

    const_iterator begin() const noexcept {
        make_contiguous();
        return const_iterator(_mem._data + _front);
    }
    const_iterator end() const noexcept {
        make_contiguous();
        return const_iterator(_mem._data + _front + _mem._size);
    }

    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }
};