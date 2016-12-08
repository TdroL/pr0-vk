#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include "swapchainOwner.hpp"

namespace rn {

namespace vlk {

class Context;

class SwapchainCreator {
public:
	SwapchainOwner create() {
		return SwapchainOwner{};
	}
};

} // vlk

} // rn
