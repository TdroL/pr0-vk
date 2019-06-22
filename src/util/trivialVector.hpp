#pragma once

#include <cmath>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>

namespace util {

template<typename T, size_t N = 4>
class TrivialVector {
public:
	// static_assert(std::is_trivially_copyable<T>::value, "Type must be trivially copyable");
	static_assert(std::is_trivially_destructible<T>::value, "Type must be trivially destructible");

	static constexpr size_t stackCapacity = std::max(1ull, N);
	size_t dataSize = 0;
	size_t dataCapacity = stackCapacity;

	using AlignedT = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

	union {
		AlignedT stack[stackCapacity];
		AlignedT *heap;
	} storage;

	TrivialVector() = default;
	TrivialVector(std::initializer_list<T> list) :
		dataSize{list.size()}
	{
		if (dataSize > dataCapacity) {
			storage.heap = new AlignedT[dataSize];

			dataCapacity = dataSize;
		}

		std::copy(std::begin(list), std::end(list), data());
	}

	TrivialVector(const TrivialVector<T, N> &other) :
		dataSize{other.dataSize}
	{
		if (dataSize > stackCapacity) {
			storage.heap = new AlignedT[dataSize];

			dataCapacity = dataSize;
		}

		std::copy(std::begin(other), std::end(other), data());
	}

	TrivialVector(TrivialVector<T, N> &&other) noexcept :
		dataSize{other.dataSize},
		dataCapacity{other.dataCapacity}
	{
		if (usesStack()) {
			std::copy(std::begin(other), std::end(other), data());
		} else {
			storage.heap = other.storage.heap;
			other.dataSize = 0;
			other.dataCapacity = stackCapacity;
		}
	}

	TrivialVector<T, N> & operator=(const TrivialVector<T, N> &other)
	{
		if (other.dataSize <= stackCapacity && ! usesStack()) {
			delete[] storage.heap;

			dataCapacity = stackCapacity;
		} else if (other.dataSize > dataCapacity) {
			if ( ! usesStack()) {
				delete[] storage.heap;
			}

			storage.heap = new AlignedT[other.dataSize];
			dataCapacity = other.dataSize;
		}

		std::copy(std::begin(other), std::end(other), data());
		dataSize = other.dataSize;

		return *this;
	}

	TrivialVector<T, N> & operator=(TrivialVector<T, N> &&other)
	{
		if (other.dataSize <= stackCapacity) {
			if ( ! usesStack()) {
				delete[] storage.heap;

				dataCapacity = stackCapacity;
			}

			std::copy(std::begin(other), std::end(other), data());
			dataSize = other.dataSize;
		} else {
			if ( ! usesStack()) {
				delete[] storage.heap;
			}

			storage.heap = other.storage.heap;
			dataSize = other.dataSize;
			dataCapacity = other.dataCapacity;

			other.dataSize = 0;
			other.dataCapacity = stackCapacity;
		}

		return *this;
	}

	virtual ~TrivialVector() {
		if ( ! usesStack()) {
			delete[] storage.heap;
		}
	}

	T & at(size_t i) {
		if (i < dataSize) {
			return data()[i];
		} else {
			throw std::out_of_range{"util::TrivialVector::at(" + std::to_string(i) + ")"};
		}
	}

	T & operator[](size_t i) {
		return data()[i];
	}

	T * data() {
		if (usesStack()) {
			return reinterpret_cast<T *>(&storage.stack[0]);
		} else {
			return reinterpret_cast<T *>(storage.heap);
		}
	}

	const T * data() const {
		if (usesStack()) {
			return reinterpret_cast<const T *>(&storage.stack[0]);
		} else {
			return reinterpret_cast<const T *>(storage.heap);
		}
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

	size_t breakpointSize() const {
		return stackCapacity;
	}

	size_t capacity() const {
		return dataCapacity;
	}

	bool usesStack() const {
		return dataCapacity <= stackCapacity;
	}

	void reserve(size_t newDataCapacity) {
		if (newDataCapacity <= dataCapacity) {
			return;
		}

		if (newDataCapacity <= stackCapacity) {
			dataCapacity = newDataCapacity;

			return;
		}

		AlignedT *tmp = new AlignedT[newDataCapacity];

		std::copy(std::begin(*this), std::end(*this), reinterpret_cast<T *>(tmp));

		if ( ! usesStack()) {
			delete[] storage.heap;
		}

		storage.heap = tmp;
		dataCapacity = newDataCapacity;
	}

	void push_back(T &&value) {
		if (dataSize >= dataCapacity) {
			reserve(static_cast<size_t>(std::ceil(1.5 * static_cast<double>(dataSize))));
		}

		new (&(data()[dataSize])) T{std::move(value)};
		dataSize++;
	}

	void push_back(const T &value) {
		push_back(std::move(value));
	}

	template<typename... Args>
	T & emplace_back(Args &&...args) {
		if (dataSize >= dataCapacity) {
			reserve(static_cast<size_t>(std::ceil(1.5 * static_cast<double>(dataSize))));
		}

		new (&(data()[dataSize])) T{std::forward<Args...>(args...)};
		dataSize++;
	}
};

} // util