#include "fs.hpp"
#include <fstream>
#include <iterator>
#include <stdexcept>

namespace ngn
{

namespace fs
{

std::vector<std::string> searchDirectories{
	"",
	"../assets/",
	"../assets/meshes/",
	"../assets/shaders/",
	"../assets/fonts/",
	"../assets/textures/",
};

// http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
template<>
std::string contents(const std::string &fileName, bool throws) {
	std::ifstream in(find(fileName), std::ios::in | std::ios::binary);

	if (in && in.good()) {
		std::string contents{};
		size_t fileSize = size(in) / sizeof(char);
		contents.reserve(fileSize);
		contents[fileSize - 1] = '\0';

		contents.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());

		return contents;
	}

	if (throws) {
		throw std::runtime_error{"ngn::fs::contents(\"" + fileName + "\") - failed to read the file"};
	}

	return std::string{};
}

template<>
std::vector<char> contents(const std::string &fileName, bool throws) {
	std::ifstream in(find(fileName), std::ios::in | std::ios::binary);

	if (in && in.good()) {
		std::vector<char> contents{};
		size_t fileSize = size(in) / sizeof(char);
		contents.reserve(fileSize);
		contents[fileSize - 1] = '\0';

		contents.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());

		return contents;
	}

	if (throws) {
		throw std::runtime_error{"ngn::fs::contents(\"" + fileName + "\") - failed to read the file"};
	}

	return std::vector<char>{};
}

std::string contents(const std::string &fileName, bool throws) {
	return contents<std::string>(fileName, throws);
}

bool contents(const std::string &fileName, const std::string &contents, bool throws) {
	std::ofstream out(find(fileName), std::ios::out | std::ios::trunc | std::ios::binary);

	if (out && out.good()) {
		out.write(contents.data(), contents.length() * sizeof(contents.data()[0]));
		return true;
	}

	if (throws) {
		throw std::runtime_error{"ngn::fs::contents(\"" + fileName + "\") - failed to write to the file"};
	}

	return false;
}

std::string find(const std::string &fileName, bool throws)
{
	for (const auto &directory : searchDirectories) {
		std::string filePath = directory + fileName;
		std::ifstream in(filePath);

		if (in.is_open()) {
			return filePath;
		}
	}

	if (throws) {
		throw std::runtime_error{"ngn::fs::find(\"" + fileName + "\") - could not find the file"};
	}

	return "";
}

std::string ext(const std::string &fileName) {
	size_t pos = fileName.find_last_of('.');

	if (pos != std::string::npos && (pos + 1) < fileName.size()) {
		return fileName.substr(pos + 1);
	}

	return "";
}

size_t size(const std::string &fileName) {
	std::ifstream in(find(fileName), std::ios::binary);

	return size(in);
}

size_t size(std::ifstream &in) {
	if (in && in.good()) {
		in.seekg(0, std::ios::end);
		size_t length = in.tellg();
		in.seekg(0, std::ios::beg);

		return length;
	}

	return -1;
}

} // fs

} // ngn