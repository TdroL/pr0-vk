#pragma once

#include <cstddef>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

namespace ngn::fs {

extern std::vector<std::string> searchDirectories;

std::optional<std::vector<std::byte>> read(std::string_view fileName, uint32_t alignment = 1);
std::optional<std::string> contents(std::string_view fileName);

std::optional<std::string> write(std::string_view fileName, const std::byte *contents, size_t size);
std::optional<std::string> write(std::string_view fileName, const std::vector<std::byte> &contents);
std::optional<std::string> write(std::string_view fileName, std::string_view contents);

std::optional<std::string> find(std::string_view fileName);

std::string ext(std::string_view fileName);

std::optional<uint64_t> size(std::string_view fileName);
uint64_t size(std::ifstream &in);

bool fileExist(std::string_view fileName);

} // ngn::fs
