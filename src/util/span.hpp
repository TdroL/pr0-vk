#pragma once

namespace util {

template<typename T>
class Span {
public:
	T *dataPtr = nullptr;
	size_t dataSize = 0;

	T * data() {
		return dataPtr;
	}

	const T * data() const {
		return dataPtr;
	}

	T * begin() {
		return data();
	}

	const T * begin() const {
		return data();
	}

	const T * cbegin() {
		return data();
	}

	T * end() {
		return &(data()[dataSize]);
	}

	const T * end() const {
		return &(data()[dataSize]);
	}

	const T * cend() {
		return &(data()[dataSize]);
	}

	size_t size() const {
		return dataSize;
	}

	bool empty() const {
		return dataSize == 0;
	}

	T & operator[](size_t index) {
		return dataPtr[index];
	}

	const T & operator[](size_t index) const {
		return dataPtr[index];
	}
};

}