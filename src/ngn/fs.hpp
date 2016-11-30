#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace ngn
{

namespace fs
{

extern std::vector<std::string> searchDirectories;

template<class T>
T contents(const std::string &fileName, bool throws = true);
std::string contents(const std::string &fileName, bool throws = true);
bool contents(const std::string &fileName, const std::string &contents, bool throws = true);
std::string find(const std::string &fileName, bool throws = true);
std::string ext(const std::string &fileName);
size_t size(const std::string &fileName);
size_t size(std::ifstream &in);

} // fs

} // ngn