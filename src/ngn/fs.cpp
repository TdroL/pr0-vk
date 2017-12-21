#include "fs.hpp"

#include <iterator>
#include <stdexcept>
#include <type_traits>

#include "../util/leadingZeroes.hpp"
#include "log.hpp"

namespace ngn::fs {

std::vector<std::string> searchDirectories{
	"./",
	"../assets/",
	"../assets/meshes/",
	"../assets/shaders/",
	"../assets/fonts/",
	"../assets/textures/",
};

uint32_t normalize(uint32_t alignment) {
	if (alignment == 0) {
		alignment = 1;
	}

	if ((alignment & (alignment - 1)) != 0) {
		alignment = 1u << (32u - util::leadingZeroes(alignment));
	}

	return alignment;
}

// http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::vector<std::byte> read(const std::string_view &fileName, uint32_t alignment, bool throws) {
	std::string filePath = find(fileName, throws);
	if (filePath.empty()) {
		ngn::log::debug("ngn::fs::read({} [{}]) => file missing", fileName.data(), filePath);

		return std::vector<std::byte>{};
	}

	std::ifstream in(filePath, std::ios::in | std::ios::binary);

	if (in && in.good()) {
		std::vector<std::byte> contents{};

		int64_t fileSize = size(in);
		if (fileSize > 0) {
			alignment = normalize(alignment);
			int64_t alignedSize = ((fileSize + alignment - 1) / alignment) * alignment;

			contents.resize(alignedSize);

			in.read(reinterpret_cast<char *>(contents.data()), fileSize);
		}

		ngn::log::debug("ngn::fs::read({} [{}]) => {} bytes", fileName.data(), filePath, contents.size());

		return contents;
	}

	if (throws) {
		throw std::runtime_error{"ngn::fs::read(\"" + std::string{fileName} + "\") - failed to read the file"};
	}

	ngn::log::debug("ngn::fs::read({} [{}]) => failed", fileName.data(), filePath);

	return std::vector<std::byte>{};
}

std::string contents(const std::string_view &fileName, bool throws) {
	std::string filePath = find(fileName, throws);
	if (filePath.empty()) {
		ngn::log::debug("ngn::fs::contents({} [{}]) => file missing", fileName.data(), filePath);

		return std::string{};
	}

	std::ifstream in(filePath, std::ios::in | std::ios::binary);

	if (in && in.good()) {
		std::string contents{};

		int64_t fileSize = size(in);
		if (fileSize > 0) {
			contents.reserve(fileSize);

			contents.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
		}

		ngn::log::debug("ngn::fs::contents({} [{}]) => {} bytes", fileName.data(), filePath, contents.size());

		return contents;
	}

	if (throws) {
		throw std::runtime_error{"ngn::fs::contents(\"" + std::string{fileName} + "\") - failed to read the file"};
	}

	ngn::log::debug("ngn::fs::contents({} [{}]) => failed", fileName.data(), filePath);

	return std::string{};
}

bool write(const std::string_view &fileName, const std::byte *contents, size_t size, bool throws) {
	std::string filePath = find(fileName, throws);
	if (filePath.empty()) {
		filePath = fileName;
	}

	std::ofstream out(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

	if (out && out.good()) {
		out.write(reinterpret_cast<const char *>(contents), size);

		ngn::log::debug("ngn::fs::write({} [{}]) => {} bytes", fileName.data(), filePath, size);

		return true;
	}

	if (throws) {
		throw std::runtime_error{"ngn::fs::write(\"" + std::string{fileName} + "\", data) - failed to write to the file"};
	}

	ngn::log::debug("ngn::fs::write({} [{}]) => failed", fileName.data(), filePath);

	return false;
}

bool write(const std::string_view &fileName, const std::vector<std::byte> &contents, bool throws) {
	return write(fileName, contents.data(), contents.size(), throws);
}

bool write(const std::string_view &fileName, const std::string_view &contents, bool throws) {
	return write(fileName, reinterpret_cast<const std::byte *>(contents.data()), contents.length() * sizeof(contents[0]), throws);
}

std::string find(const std::string_view &fileName, bool throws) {
	const std::string fileNameStr{fileName};

	for (const auto &directory : searchDirectories) {
		std::string filePath = directory + fileNameStr;

		if (std::ifstream{filePath}.is_open()) {
			return filePath;
		}
	}

	if (throws) {
		throw std::runtime_error{"ngn::fs::find(\"" + fileNameStr + "\") - could not find the file"};
	}

	return "";
}

std::string ext(const std::string_view &fileName) {
	size_t pos = fileName.find_last_of('.');

	if (pos != std::string::npos && (pos + 1) < fileName.size()) {
		return std::string{fileName.substr(pos + 1)};
	}

	return "";
}

int64_t size(const std::string_view &fileName) {
	std::ifstream in(find(fileName), std::ios::ate | std::ios::binary);

	return static_cast<int64_t>(in.tellg());
}

int64_t size(std::ifstream &in) {
	if (in && in.good()) {
		in.seekg(0, std::ios::end);
		int64_t length = static_cast<int64_t>(in.tellg());
		in.seekg(0, std::ios::beg);

		return length;
	}

	return -1;
}

} // ngn::fs
