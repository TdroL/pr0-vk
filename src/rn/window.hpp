#pragma once

#include <memory>
#include <optional>

#include "../ngn/config.hpp"
#include "types.hpp"

namespace rn {

class Window {
public:
	struct Handle;

	static void destroy(Handle *handle);

	static void onResize(Handle *handle, int width, int height);

	std::unique_ptr<Handle, decltype(&Window::destroy)> handle;

	struct Properties {
		std::string title{""};
		uint32_t width{0};
		uint32_t height{0};
		std::optional<uint32_t> monitor{std::nullopt};
		rn::WindowMode mode{rn::WindowMode::Windowed};
	};

	Properties currentProperties{};

	Properties nextProperties{};

	explicit Window(Properties &&initialProperties) noexcept;

	void loadState();

	bool needsRefresh();

	void refresh();

	void requestClose();
	void forceClose();
	bool shouldClose();

	void pollEvents();

	void create();
};

}
