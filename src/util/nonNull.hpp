#pragma once

namespace util {

template<class T>
class NonNull {
public:
	T *ptr;

	NonNull(T &value) noexcept :
		ptr{&value}
	{}

	NonNull(const NonNull &other) noexcept :
		ptr{other.ptr}
	{}

	NonNull(NonNull &&other) noexcept :
		ptr{other.ptr}
	{}

	NonNull & operator=(T &value) {
		ptr = &value;

		return *this;
	}

	NonNull & operator=(const NonNull &other) {
		ptr = other.ptr;

		return *this;
	}

	NonNull & operator=(NonNull &&other) {
		ptr = other.ptr;

		return *this;
	}

	T & operator*()  const { return *ptr; }
	T * operator->() const { return ptr; }

	T * data() {
		return ptr;
	}
};

} // util
