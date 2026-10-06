#pragma once
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "tmathvector.h"

template <class T>
class TMatrix : public TMathVector<TMathVector<T>> {
public:
	explicit TMatrix(size_t rows = 0, size_t cols = 0);
	TMatrix(std::initializer_list<std::initializer_list<T>> list);
	TMatrix(const TMathVector<TMathVector<T>>& other);

	size_t rows() const noexcept;
	size_t cols() const noexcept;

	TMatrix operator+(const TMatrix& other) const;
	TMatrix operator-(const TMatrix& other) const;
	TMatrix operator-() const;
	TMatrix operator*(const T& scalar) const;
	TMatrix operator/(const T& scalar) const;
	TMatrix operator*(const TMatrix& other) const;
	TMathVector<T> operator*(const TMathVector<T>& v) const;

	TMatrix transposed() const;
	double determinant() const;
	TMatrix<double> inverse() const;

	static TMatrix identity(size_t n);
};

template <class T>
TMatrix<T> operator*(const T& scalar, const TMatrix<T>& m);

template <class T>
std::ostream& operator<<(std::ostream& os, const TMatrix<T>& m);

template <class T>
TMatrix<T>::TMatrix(size_t rows, size_t cols) : TMathVector<TMathVector<T>>(rows) {
	for (size_t i = 0; i < rows; i++) {
		(*this)[i] = TMathVector<T>(cols);
	}
}

template <class T>
TMatrix<T>::TMatrix(std::initializer_list<std::initializer_list<T>> list) {
	size_t width = list.size() ? list.begin()->size() : 0;
	for (const std::initializer_list<T>& row : list) {
		if (row.size() != width) throw std::invalid_argument("rows have different lengths");
		this->push_back(TMathVector<T>(row.begin(), row.size()));
	}
}

template <class T>
TMatrix<T>::TMatrix(const TMathVector<TMathVector<T>>& other) : TMathVector<TMathVector<T>>(other) {
	for (size_t i = 1; i < this->size(); i++) {
		if ((*this)[i].size() != (*this)[0].size()) throw std::invalid_argument("rows have different lengths");
	}
}

template <class T>
size_t TMatrix<T>::rows() const noexcept {
	return this->size();
}

template <class T>
size_t TMatrix<T>::cols() const noexcept {
	return this->size() == 0 ? 0 : (*this)[0].size();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const TMatrix<T>& m) {
	for (size_t i = 0; i < m.rows(); i++) {
		os << m[i];
		if (i + 1 < m.rows()) os << "\n";
	}
	return os;
}

template <class T>
TMatrix<T> TMatrix<T>::operator+(const TMatrix& other) const {
	TMatrix result(*this);
	result += other;
	return result;
}

template <class T>
TMatrix<T> TMatrix<T>::operator-(const TMatrix& other) const {
	TMatrix result(*this);
	result -= other;
	return result;
}

template <class T>
TMatrix<T> TMatrix<T>::operator-() const {
	TMatrix result(*this);
	for (size_t i = 0; i < result.rows(); i++) {
		result[i] = -result[i];
	}
	return result;
}

template <class T>
TMatrix<T> TMatrix<T>::operator*(const T& scalar) const {
	TMatrix result(*this);
	for (size_t i = 0; i < result.rows(); i++) {
		result[i] *= scalar;
	}
	return result;
}

template <class T>
TMatrix<T> TMatrix<T>::operator/(const T& scalar) const {
	TMatrix result(*this);
	for (size_t i = 0; i < result.rows(); i++) {
		result[i] /= scalar;
	}
	return result;
}

template <class T>
TMatrix<T> TMatrix<T>::operator*(const TMatrix& other) const {
	if (cols() != other.rows()) throw std::invalid_argument("matrix sizes do not match");

	TMatrix result(rows(), other.cols());
	for (size_t i = 0; i < rows(); i++) {
		for (size_t j = 0; j < other.cols(); j++) {
			T sum = T();
			for (size_t k = 0; k < cols(); k++) {
				sum += (*this)[i][k] * other[k][j];
			}
			result[i][j] = sum;
		}
	}
	return result;
}

template <class T>
TMathVector<T> TMatrix<T>::operator*(const TMathVector<T>& v) const {
	if (cols() != v.size()) throw std::invalid_argument("matrix and vector sizes do not match");

	TMathVector<T> result(rows());
	for (size_t i = 0; i < rows(); i++) {
		result[i] = (*this)[i] * v;
	}
	return result;
}

template <class T>
TMatrix<T> TMatrix<T>::transposed() const {
	TMatrix result(cols(), rows());
	for (size_t i = 0; i < rows(); i++) {
		for (size_t j = 0; j < cols(); j++) {
			result[j][i] = (*this)[i][j];
		}
	}
	return result;
}

template <class T>
double TMatrix<T>::determinant() const {
	size_t n = rows();
	if (n != cols()) throw std::invalid_argument("matrix is not square");

	TMatrix<double> m(n, n);
	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < n; j++) {
			m[i][j] = static_cast<double>((*this)[i][j]);
		}
	}

	double det = 1.0;
	for (size_t c = 0; c < n; c++) {
		size_t p = c;
		for (size_t r = c + 1; r < n; r++) {
			if (std::fabs(m[r][c]) > std::fabs(m[p][c])) p = r;
		}
		if (std::fabs(m[p][c]) < 1e-12) return 0.0;
		if (p != c) {
			std::swap(m[p], m[c]);
			det = -det;
		}
		det *= m[c][c];
		for (size_t r = c + 1; r < n; r++) {
			double f = m[r][c] / m[c][c];
			for (size_t k = c; k < n; k++) {
				m[r][k] -= f * m[c][k];
			}
		}
	}
	return det;
}

template <class T>
TMatrix<double> TMatrix<T>::inverse() const {
	size_t n = rows();
	if (n != cols()) throw std::invalid_argument("matrix is not square");

	TMatrix<double> a(n, n);
	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < n; j++) {
			a[i][j] = static_cast<double>((*this)[i][j]);
		}
	}
	TMatrix<double> inv = TMatrix<double>::identity(n);

	for (size_t c = 0; c < n; c++) {
		size_t p = c;
		for (size_t r = c + 1; r < n; r++) {
			if (std::fabs(a[r][c]) > std::fabs(a[p][c])) p = r;
		}
		if (std::fabs(a[p][c]) < 1e-12) throw std::runtime_error("matrix is singular");
		std::swap(a[p], a[c]);
		std::swap(inv[p], inv[c]);

		double d = a[c][c];
		for (size_t k = 0; k < n; k++) {
			a[c][k] /= d;
			inv[c][k] /= d;
		}
		for (size_t r = 0; r < n; r++) {
			if (r == c) continue;
			double f = a[r][c];
			for (size_t k = 0; k < n; k++) {
				a[r][k] -= f * a[c][k];
				inv[r][k] -= f * inv[c][k];
			}
		}
	}
	return inv;
}

template <class T>
TMatrix<T> TMatrix<T>::identity(size_t n) {
	TMatrix result(n, n);
	for (size_t i = 0; i < n; i++) {
		result[i][i] = T(1);
	}
	return result;
}

template <class T>
TMatrix<T> operator*(const T& scalar, const TMatrix<T>& m) {
	return m * scalar;
}