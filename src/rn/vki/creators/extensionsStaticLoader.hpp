#pragma once

namespace rn::vki {

class Context;

class ExtensionsStaticLoader {
public:
	void loadInstance(Context &context);
	void loadDevice(Context &context);
};

} // rn::vki
