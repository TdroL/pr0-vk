#pragma once

#include <string>
#include <vector>

#include "../../ngn/log.hpp"
#include "../resources.hpp"
#include "../types.hpp"
#include "buffer.hpp"
#include "texture.hpp"
#include "types.hpp"

namespace rn::db {

template<class T>
struct Model {
	std::string filePath{};

	std::vector<rn::db::Texture<T>> textures{};
	std::vector<rn::db::Buffer<T>> buffers{};

	rn::db::ModelState state{rn::db::ModelState::Empty};
	rn::FenceStamp latestFenceStamp{rn::end<rn::FenceStamp>()};

	Model() = default;

	Model(Model &&other) :
		filePath{std::move(other.filePath)},
		textures{std::move(other.textures)},
		buffers{std::move(other.buffers)},
		state{std::move(other.state)},
		latestFenceStamp{std::move(other.latestFenceStamp)}
	{
		other.state = rn::db::ModelState::Empty;
		other.latestFenceStamp = rn::end<rn::FenceStamp>();
	}

	Model<T> & operator=(Model &&other) {
		filePath = std::move(other.filePath);
		textures = std::move(other.textures);
		buffers = std::move(other.buffers);
		state = std::move(other.state);
		latestFenceStamp = std::move(other.latestFenceStamp);

		other.state = rn::db::ModelState::Empty;
		other.latestFenceStamp = rn::end<rn::FenceStamp>();

		return *this;
	}

	Model(const Model &other) = delete;
	Model<T> & operator=(const Model &other) = delete;

	Model(std::string &&filePath) :
		filePath{std::move(filePath)},
		state{rn::db::ModelState::Initialized}
	{}

	Model(std::string &&filePath, std::vector<rn::db::Texture<T>> &&textures, std::vector<rn::db::Buffer<T>> &&buffers) :
		filePath{std::move(filePath)},
		textures{std::move(textures)},
		buffers{std::move(buffers)},
		state{rn::db::ModelState::Loading}
	{
		for (const auto &texture : textures) {
			if (latestFenceStamp == rn::end<rn::FenceStamp>() || latestFenceStamp < texture.fenceStamp) {
				latestFenceStamp = texture.fenceStamp;
			}
		}

		for (const auto &buffer : buffers) {
			if (latestFenceStamp == rn::end<rn::FenceStamp>() || latestFenceStamp < buffer.fenceStamp) {
				latestFenceStamp = buffer.fenceStamp;
			}
		}
	}

	bool isEmpty() const {
		return state == rn::db::ModelState::Empty;
	}

	bool isInitialized() const {
		return state == rn::db::ModelState::Initialized;
	}

	bool isLoading() const {
		return state == rn::db::ModelState::Loading;
	}

	bool isReady() const {
		return state == rn::db::ModelState::Ready;
	}

	bool advanceIfTransferred(rn::FenceStamp fenceStamp) {
		if (latestFenceStamp < fenceStamp) {
			state = rn::db::ModelState::Ready;

			return true;
		}

		return false;
	}

	void reset() {
		// if ( ! handles.buffers.empty()) {
		if ( ! buffers.empty()) {
			ngn::log::debug("rn::db::Model::reset() => unload model loaded from file \"{}\"", filePath);
		}

		textures.clear();
		buffers.clear();

		state = rn::db::ModelState::Empty;
		latestFenceStamp = rn::end<rn::FenceStamp>();

		// for (auto &&texture : handles.textures) {
		// 	resources->texture.retire(texture);
		// }

		// for (auto &&buffer : handles.buffers) {
		// 	resources->buffer.retire(buffer);
		// }

		// handles = {};
		// fenceStamps = {};
	}

	~Model() {
		reset();
	}
};

} // rn::db