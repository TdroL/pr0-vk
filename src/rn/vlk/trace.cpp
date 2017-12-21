#include "trace.hpp"

#include <map>
// #include <mutex>
#include <thread>

namespace rn::vlk {

std::map<std::thread::id, LastCall> stores{};

uint32_t baseOffset = std::string_view{__FILE__}.length() - std::string_view{"src/rn/vlk/trace.cpp"}.length();

void trace(uint32_t line, const std::string_view &file, const std::string_view &code) {
	LastCall store;
	store.line = line;
	store.file = file.substr(baseOffset);
	store.code = code;

	stores.insert_or_assign(std::this_thread::get_id(), store);
}

LastCall & lastCall() {
	return stores[std::this_thread::get_id()];
}

} // rn::vlk
