#pragma once

namespace util {

template<class... Ts> struct fold : Ts... { using Ts::operator()...; };
template<class... Ts> fold(Ts...) -> fold<Ts...>;

}
