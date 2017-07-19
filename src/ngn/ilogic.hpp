#pragma once

namespace ngn {

class ILogic {
public:
	virtual ~ILogic() {}
	virtual void onInit() = 0;
	virtual void onResize() = 0;

	virtual void update() = 0;
	virtual void render() = 0;
};

} // ngn
