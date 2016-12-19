#pragma once

#include "ilogic.hpp"

namespace ngn {

class Game {
public:
	std::unique_ptr<ILogic> logic;
};

} // ngn