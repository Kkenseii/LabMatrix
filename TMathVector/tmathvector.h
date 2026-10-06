#pragma once
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "tvector.h"

template <class T>
class TMathVector : public TVector<T> {
public:
	explicit TMathVector(size_t size = 0);
	TMathVector(std::initializer_list<T> list);
	TMathVector(const T* arr, size_t size);
	TMathVector(const TVector<T>& other);

	TMathVector& operator+=(const TMathVector& other);
	TMathVector& operator-=(const TMathVector& other);
	TMathVector& operator*=(const T& scalar);
	TMathVector& operator/=(const T& scalar);

	TMathVector operator+(const TMathVector& other) const;
	TMathVector operator-(const TMathVector& other) const;
	TMathVector operator-() const;
	TMathVector operator*(const T& scalar) const;
	TMathVector operator/(const T& scalar) const;

	T operator*(const TMathVector& other) const;
	double length() const;

	bool operator==(const TMathVector& other) const;
	bool operator!=(const TMathVector& other) const;
};

template <class T>
TMathVector<T> operator*(const T& scalar, const TMathVector<T>& v);

template <class T>
std::ostream& operator<<(std::ostream& os, const TMathVector<T>& v);

template <class T>
TMathVector<T>::TMathVector(size_t size) : TVector<T>(size) {}

template <class T>
TMathVector<T>::TMathVector(std::initializer_list<T> list) : TVector<T>(list) {}

template <class T>
TMathVector<T>::TMathVector(const T* arr, size_t size) : TVector<T>(arr, size) {}

template <class T>
TMathVector<T>::TMathVector(const TVector<T>& other) : TVector<T>(other) {}

template <class T>
TMathVector<T>& TMathVector<T>::operator+=(const TMathVector& other) {
	if (this->size() != other.size()) throw std::invalid_argument("vector sizes differ");
	for (size_t i = 0; i < this->size(); i++) {
		(*this)[i] += other[i];
	}
	return *this;
}

template <class T>
TMathVector<T>& TMathVector<T>::operator-=(const TMathVector& other) {
	if (this->size() != other.size()) throw std::invalid_argument("vector sizes differ");
	for (size_t i = 0; i < this->size(); i++) {
		(*this)[i] -= other[i];
	}
	return *this;
}

template <class T>
TMathVector<T>& TMathVector<T>::operator*=(const T& scalar) {
	for (size_t i = 0; i < this->size(); i++) {
		(*this)[i] *= scalar;
	}
	return *this;
}

template <class T>
TMathVector<T>& TMathVector<T>::operator/=(const T& scalar) {
	if (scalar == T()) throw std::invalid_argument("division by zero");
	for (size_t i = 0; i < this->size(); i++) {
		(*this)[i] /= scalar;
	}
	return *this;
}

template <class T>
TMathVector<T> TMathVector<T>::operator+(const TMathVector& other) const {
	TMathVector result(*this);
	result += other;
	return result;
}

template <class T>
TMathVector<T> TMathVector<T>::operator-(const TMathVector& other) const {
	TMathVector result(*this);
	result -= other;
	return result;
}

template <class T>
TMathVector<T> TMathVector<T>::operator-() const {
	TMathVector result(*this);
	for (size_t i = 0; i < result.size(); i++) {
		result[i] = -result[i];
	}
	return result;
}

template <class T>
TMathVector<T> TMathVector<T>::operator*(const T& scalar) const {
	TMathVector result(*this);
	result *= scalar;
	return result;
}

template <class T>
TMathVector<T> TMathVector<T>::operator/(const T& scalar) const {
	TMathVector result(*this);
	result /= scalar;
	return result;
}

template <class T>
T TMathVector<T>::operator*(const TMathVector& other) const {
	if (this->size() != other.size()) throw std::invalid_argument("vector sizes differ");
	T result = T();
	for (size_t i = 0; i < this->size(); i++) {
		result += (*this)[i] * other[i];
	}
	return result;
}

template <class T>
double TMathVector<T>::length() const {
	return std::sqrt(static_cast<double>((*this) * (*this)));
}

template <class T>
bool TMathVector<T>::operator==(const TMathVector& other) const {
	if (this->size() != other.size()) return false;
	for (size_t i = 0; i < this->size(); i++) {
		if (!((*this)[i] == other[i])) return false;
	}
	return true;
}

template <class T>
bool TMathVector<T>::operator!=(const TMathVector& other) const {
	return !(*this == other);
}

template <class T>
TMathVector<T> operator*(const T& scalar, const TMathVector<T>& v) {
	return v * scalar;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const TMathVector<T>& v) {
	os << "[ ";
	for (size_t i = 0; i < v.size(); i++) {
		os << v[i] << " ";
	}
	os << "]";
	return os;
}