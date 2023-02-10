#pragma once

#include <fstream>
#include <filesystem>

namespace util {

inline std::optional<std::vector<std::byte>> fsReadBytes(const std::filesystem::path &filePath) {
	std::ifstream in{filePath, std::ios::in | std::ios::binary};

	if ( ! in || ! in.good()) {
		return std::nullopt;
	}

	in.seekg(0, std::ios::end);
	auto fileSize = in.tellg();
	if (fileSize < 0) {
		return std::nullopt;
	}

	std::vector<std::byte> storage{};
	storage.resize(fileSize);

	in.seekg(0, std::ios::beg);
	in.read(reinterpret_cast<char *>(storage.data()), fileSize);

	return storage;
}

} // util
