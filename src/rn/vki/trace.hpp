#pragma once

#include <string_view>
#include <functional>

#define RN_VKI_TRACE(code) std::invoke([&] () { rn::vki::trace(__LINE__, __FILE__, #code); return (code); })

namespace rn::vki {

struct LastCall {
	uint32_t line{0};
	std::string_view file{};
	std::string_view code{};
};

void trace(uint32_t line, const std::string_view &file, const std::string_view &code);

LastCall & traceLastCall();

} // rn::vki
