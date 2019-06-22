#include "trace.hpp"

#include <map>
// #include <mutex>
#include <thread>

namespace rn::vki {

std::map<std::thread::id, LastCall> stores{};

size_t baseOffset = std::string_view{__FILE__}.length() - std::string_view{"src/rn/vki/trace.cpp"}.length();

void trace(uint32_t line, const std::string_view &file, const std::string_view &code) {
	LastCall store;
	store.line = line;
	store.file = file.substr(baseOffset);
	store.code = code;

	stores.insert_or_assign(std::this_thread::get_id(), store);
}

LastCall & traceLastCall() {
	return stores[std::this_thread::get_id()];
}

} // rn::vki
