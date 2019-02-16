#pragma once

#include <exception>

namespace util {

template <typename F>
class ScopeExit {
public:
	F fn;

	explicit ScopeExit(F fn)
		: fn{std::move(fn)}
	{}

	ScopeExit() = delete;
	ScopeExit(const ScopeExit &) = delete;
	ScopeExit(ScopeExit &&) = delete;

	ScopeExit & operator=(const ScopeExit &) = delete;
	ScopeExit & operator=(ScopeExit &&) = delete;

	~ScopeExit() {
		fn();
	}
};

template <typename F>
class ScopeSuccess {
public:
	F fn;
	int exceptionCount;

	explicit ScopeSuccess(F fn)
		: fn{std::move(fn)}, exceptionCount(std::uncaught_exceptions())
	{}

	ScopeSuccess() = delete;
	ScopeSuccess(const ScopeSuccess &) = delete;
	ScopeSuccess(ScopeSuccess &&) = delete;

	ScopeSuccess & operator=(const ScopeSuccess &) = delete;
	ScopeSuccess & operator=(ScopeSuccess &&) = delete;

	~ScopeSuccess() {
		if (exceptionCount == std::uncaught_exceptions()) {
			fn();
		}
	}
};

template <typename F>
class ScopeFail {
public:
	F fn;
	int exceptionCount;

	explicit ScopeFail(F fn)
		: fn{std::move(fn)}, exceptionCount(std::uncaught_exceptions())
	{}

	ScopeFail() = delete;
	ScopeFail(const ScopeFail &) = delete;
	ScopeFail(ScopeFail &&) = delete;

	ScopeFail & operator=(const ScopeFail &) = delete;
	ScopeFail & operator=(ScopeFail &&) = delete;

	~ScopeFail() {
		if (exceptionCount < std::uncaught_exceptions()) {
			fn();
		}
	}
};

template <class F>
inline ScopeExit<F> createScopeExit(const F& f) noexcept {
	return ScopeExit<F>(f);
}

template <class F>
inline ScopeExit<F> createScopeExit(F&& f) noexcept {
	return ScopeExit<F>(std::forward<F>(f));
}

template <class E, class F>
inline ScopeExit<F> createScopeExit(const E& e, const F& f) noexcept {
	e();

	return ScopeExit<F>(f);
}

template <class E, class F>
inline ScopeExit<F> createScopeExit(E&& e, F&& f) noexcept {
	e();

	return ScopeExit<F>(std::forward<F>(f));
}

template <class F>
inline ScopeSuccess<F> createScopeSuccess(const F& f) noexcept {
	return ScopeSuccess<F>(f);
}

template <class F>
inline ScopeSuccess<F> createScopeSuccess(F&& f) noexcept {
	return ScopeSuccess<F>(std::forward<F>(f));
}

template <class F>
inline ScopeFail<F> createScopeFail(const F& f) noexcept {
	return ScopeFail<F>(f);
}

template <class F>
inline ScopeFail<F> createScopeFail(F&& f) noexcept {
	return ScopeFail<F>(std::forward<F>(f));
}

} // util
