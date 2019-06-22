#include "fs.hpp"

#include <filesystem>
#include <iterator>
#include <stdexcept>
#include <type_traits>

#include "../util/leadingZeroes.hpp"
#include "log.hpp"

namespace ngn::fs {

std::vector<std::string> searchDirectories{
	std::filesystem::path{"."}.string(),
	std::filesystem::path{".."}.string(),
	// "../assets/",
	// "../assets/meshes/",
	// "../assets/models/",
	// "../assets/shaders/",
	// "../assets/fonts/",
	// "../assets/textures/",
};

uint32_t alignToPowerOfTwo(uint32_t alignment) {
	if (alignment == 0) {
		alignment = 1;
	}

	if ((alignment & (alignment - 1)) != 0) {
		alignment = 1u << (32u - util::leadingZeroes(alignment));
	}

	return alignment;
}

// http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::optional<std::vector<std::byte>> read(std::string_view fileName, uint32_t alignment) {
	std::optional<std::string> filePathO = find(fileName);
	if ( ! filePathO) {
		ngn::log::debug("ngn::fs::read({}) => file missing", fileName.data());

		return std::nullopt;
	}

	std::ifstream in(filePathO.value(), std::ios::in | std::ios::binary);

	if (in && in.good()) {
		std::vector<std::byte> contentsStorage{};
		uint64_t fileSize = size(in);

		if (fileSize > 0) {
			alignment = alignToPowerOfTwo(alignment);
			uint64_t alignedSize = ((fileSize + alignment - 1) / alignment) * alignment;

			contentsStorage.resize(alignedSize);

			in.read(reinterpret_cast<char *>(contentsStorage.data()), fileSize);
		}

		ngn::log::debug("ngn::fs::read({} [{}]) => {} bytes", fileName.data(), filePathO.value(), contentsStorage.size());

		return {contentsStorage};
	}

	ngn::log::debug("ngn::fs::read({} [{}]) => failed", fileName.data(), filePathO.value());

	return std::nullopt;
}

std::optional<std::string> contents(std::string_view fileName) {
	std::optional<std::string> filePathO = find(fileName);
	if ( ! filePathO) {
		ngn::log::debug("ngn::fs::contents({}) => file missing", fileName.data());

		return std::nullopt;
	}

	std::ifstream in(filePathO.value(), std::ios::in | std::ios::binary);

	if (in && in.good()) {
		std::string contentsStorage{};
		uint64_t fileSize = size(in);

		if (fileSize > 0) {
			contentsStorage.reserve(fileSize);

			contentsStorage.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
		}

		ngn::log::debug("ngn::fs::contents({} [{}]) => {} bytes", fileName.data(), filePathO.value(), contentsStorage.size());

		return {contentsStorage};
	}

	ngn::log::debug("ngn::fs::contents({} [{}]) => failed", fileName.data(), filePathO.value());

	return std::nullopt;
}

std::optional<std::string> write(std::string_view fileName, const std::byte *contents, size_t size) {
	std::optional<std::string> filePathO = find(fileName);
	if ( ! filePathO) {
		filePathO = fileName;
	}

	std::ofstream out(filePathO.value(), std::ios::out | std::ios::trunc | std::ios::binary);

	if (out && out.good()) {
		out.write(reinterpret_cast<const char *>(contents), size);

		ngn::log::debug("ngn::fs::write({} [{}]) => {} bytes", fileName.data(), filePathO.value(), size);

		return filePathO;
	}

	ngn::log::debug("ngn::fs::write({} [{}]) => failed", fileName.data(), filePathO.value());

	return std::nullopt;
}

std::optional<std::string> write(std::string_view fileName, const std::vector<std::byte> &contents) {
	return write(fileName, contents.data(), contents.size());
}

std::optional<std::string> write(std::string_view fileName, std::string_view contents) {
	return write(fileName, reinterpret_cast<const std::byte *>(contents.data()), contents.length() * sizeof(contents[0]));
}

std::optional<std::string> find(std::string_view fileName) {
	// const std::string fileNameStr{fileName};

	for (const auto &directory : searchDirectories) {
		auto filePath = std::filesystem::path(directory) / fileName;

		if (std::filesystem::exists(filePath)) {
			return filePath.string();
		}
	}

	return std::nullopt;
}

std::string base(std::string_view filePath) {
	return std::filesystem::path{filePath}.parent_path().string();
}

std::string ext(std::string_view fileName) {
	return std::filesystem::path{fileName}.extension().string();
	// size_t pos = fileName.find_last_of('.');

	// if (pos != std::string::npos && (pos + 1u) < fileName.size()) {
	// 	return std::string{fileName.substr(pos + 1u)};
	// }

	// return "";
}

std::optional<uint64_t> size(std::string_view fileName) {
	std::optional<std::string> filePathO = find(fileName);
	if ( ! filePathO) {
		ngn::log::debug("ngn::fs::size({}) => file missing", fileName.data());

		return std::nullopt;
	}

	return static_cast<uint64_t>(std::filesystem::file_size(filePathO.value()));
	// std::ifstream in(filePathO.value(), std::ios::ate | std::ios::binary);

	// return {static_cast<uint64_t>(in.tellg())};
}

uint64_t size(std::ifstream &in) {
	if (in && in.good()) {
		in.seekg(0, std::ios::end);
		int64_t length = static_cast<int64_t>(in.tellg());
		in.seekg(0, std::ios::beg);

		return length;
	}

	return 0;
}

bool fileExist(std::string_view fileName) {
	return std::filesystem::exists(fileName);
	// std::ifstream file(fileName.data());
	// return file.good();
}

} // ngn::fs
