#pragma once

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

namespace ngn::fs {

extern std::vector<std::string> searchDirectories;

std::vector<std::byte> read(const std::string_view &fileName, uint32_t alignment = 1, bool throws = true);
std::string contents(const std::string_view &fileName, bool throws = true);

bool write(const std::string_view &fileName, const std::byte *contents, size_t size, bool throws = true);
bool write(const std::string_view &fileName, const std::vector<std::byte> &contents, bool throws = true);
bool write(const std::string_view &fileName, const std::string_view &contents, bool throws = true);

std::string find(const std::string_view &fileName, bool throws = true);

std::string ext(const std::string_view &fileName);

int64_t size(const std::string_view &fileName);
int64_t size(std::ifstream &in);

} // ngn::fs
