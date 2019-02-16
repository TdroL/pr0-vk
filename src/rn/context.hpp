#pragma once

namespace rn {

template<class T>
class Context {
public:
  T internal{};

  Context() = default;

  explicit Context(T &&internal) :
    internal{std::move(internal)}
  {}

  Context(const Context &other) = delete;
  Context(Context &&other) = default;

  Context & operator=(T &&other) {
    internal = std::move(other);

    return *this;
  }

  Context & operator=(const Context &other) = delete;
  Context & operator=(Context &&other) = default;
};

} // rn