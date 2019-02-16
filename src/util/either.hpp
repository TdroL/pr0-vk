#pragma once

#include <optional>
#include <variant>

namespace util {

template<class L, class R>
class EitherOption {
public:
  EitherOption() = delete;

	EitherOption(EitherOption &&other) noexcept :
		leftOpt{std::move(other.leftOpt)},
		rightOpt{std::move(other.rightOpt)}
	{}

	EitherOption(L &&leftValue) noexcept :
		leftOpt{std::move(leftValue)},
    rightOpt{std::nullopt}
	{}

	EitherOption(R &&rightValue) noexcept :
		leftOpt{std::nullopt},
    rightOpt{std::move(rightValue)}
	{}

	EitherOption & operator=(EitherOption &&other) {
		leftOpt = std::move(other.leftOpt);
		rightOpt = std::move(other.rightOpt);

		return *this;
	}

	EitherOption & operator=(L &&leftValue) {
		leftOpt = std::move(leftValue);
		rightOpt = std::nullopt;

		return *this;
	}

	EitherOption & operator=(R &&rightValue) {
		leftOpt = std::nullopt;
		rightOpt = std::move(rightValue);

		return *this;
	}

	bool isLeft() const {
		return leftOpt.has_value();
	}

	bool isRight() const {
		return rightOpt.has_value();
	}

	const L & left() const {
		return leftOpt.value();
	}

	const R & right() const {
		return rightOpt.value();
	}

	L & left() {
		return leftOpt.value();
	}

	R & right() {
		return rightOpt.value();
	}
private:
	std::optional<L> leftOpt;
	std::optional<R> rightOpt;
};

template<class L, class R>
class EitherVariant {
public:
  EitherVariant() = delete;

	explicit EitherVariant(EitherVariant &&other) noexcept :
		values{std::move(other.values)}
	{}

	EitherVariant(L &&leftValue) noexcept :
		values{std::in_place_type_t<L>{}, std::move(leftValue)}
	{}

	EitherVariant(R &&rightValue) noexcept :
		values{std::in_place_type_t<R>{}, std::move(rightValue)}
	{}

	EitherVariant & operator=(EitherVariant &&other) {
		values = std::move(other.values);

		return *this;
	}

	EitherVariant & operator=(L &&leftValue) {
		values = {std::in_place_type_t<L>{}, std::move(leftValue)};

		return *this;
	}

	EitherVariant & operator=(R &&rightValue) {
		values = {std::in_place_type_t<R>{}, std::move(rightValue)};

		return *this;
	}

	bool isLeft() const {
		return values.index() == 0;
	}

	bool isRight() const {
		return values.index() == 1;
	}

	const L & left() const {
		return std::get<0>(values);
	}

	const R & right() const {
		return std::get<1>(values);
	}

	L & left() {
		return std::get<0>(values);
	}

	R & right() {
		return std::get<1>(values);
	}
private:
	std::variant<L, R> values;
};

} // util