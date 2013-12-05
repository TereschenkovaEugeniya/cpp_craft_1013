#ifndef _TASK5_5_VECTOR_H_
#define _TASK5_5_VECTOR_H_

#include <cstdlib>
#include <vector>
#include <stdexcept>

namespace task5_5 {
template<typename T>
class vector {
public:
	typedef T* iterator; // you could change this
	typedef const T* const_iterator; // you could change this
public:
	explicit vector();
	vector(const vector& copy);
	vector& operator=(const vector& copy_from);

	void push_back(const T& value);
	void insert(const size_t index, const T& value);

	T& operator[](const size_t index);
	const T& operator[](const size_t index) const;

	void resize(const size_t amount);
	void reserve(const size_t amount);

	size_t size() const;
	size_t capacity() const;
	bool empty() const;

	iterator begin();
	iterator end();

	const_iterator begin() const;
	const_iterator end() const;

private:
	std::vector<T> v_;
	iterator itNull;
};

template<typename T>
vector<T>::vector() :
		itNull(0) {
	v_.reserve(4);
}
template<typename T>
vector<T>::vector(const vector<T>& that) :
		itNull(0) {
	v_.clear();
	for (vector<T>::const_iterator it = that.begin(); it != that.end(); ++it) {
		v_.push_back(*it);
	}
}
template<typename T>
vector<T>& vector<T>::operator=(const vector<T>& that) {
	v_.clear();
	for (vector<T>::const_iterator it = that.begin(); it != that.end(); ++it) {
		v_.push_back(*it);
	}

	return *this;
}

template<typename T>
void vector<T>::push_back(const T& value) {
	v_.push_back(value);
}

template<typename T>
void vector<T>::insert(const size_t position, const T& value) {
	v_.insert(v_.begin() + position, value);
}

template<typename T>
T& vector<T>::operator[](const size_t index) {
	if (index < v_.size()) {
		return v_[index];
	}

	throw std::out_of_range("wrong vector index");
}

template<typename T>
const T& vector<T>::operator[](const size_t index) const {
	if (index < v_.size()) {
		return v_[index];
	}

	throw std::out_of_range("wrong vector index");
}

template<typename T>
void vector<T>::resize(const size_t size) {
	v_.resize(size);
}
template<typename T>
void vector<T>::reserve(const size_t size) {
	v_.reserve(size);
}

template<typename T>
size_t vector<T>::size() const {
	return v_.size();
}
template<typename T>
size_t vector<T>::capacity() const {
	return v_.capacity();
}
template<typename T>
bool vector<T>::empty() const {
	return false;
}
template<typename T>
typename vector<T>::iterator vector<T>::begin() {
	if (v_.size() == 0) {
		return itNull;
	}
	return &(v_[0]);
}
template<typename T>
typename vector<T>::iterator vector<T>::end() {
	if (v_.size() == 0) {
		return itNull;
	}
	return &(v_[0]) + v_.size();
}

template<typename T>
typename vector<T>::const_iterator vector<T>::begin() const {
	if (v_.size() == 0) {
		return itNull;
	}
	return &(v_[0]);
}
template<typename T>
typename vector<T>::const_iterator vector<T>::end() const {
	if (v_.size() == 0) {
		return itNull;
	}
	return &(v_[0]) + v_.size();
}
}

#endif // _TASK5_5_VECTOR_H_

