#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>
#include <iostream>

using namespace std;

int main() {
	cout << "vkCreateInstance=" << reinterpret_cast<void*>(vkCreateInstance) << endl;
	return 0;
}