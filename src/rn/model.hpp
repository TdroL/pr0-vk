#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>

namespace rn {

struct Model {
	struct Flags {
		enum Enum {
			Geometry_Position   = 1<<0, // Vec3f
			Geometry_Normal     = 1<<1, // Vec3f
			Geometry_TexCoord0  = 1<<2, // Vec2f
			Material            = 1<<10,
			ComputeBounds       = 1<<11,
			All = 0xFFFFFFFF
		};
	};

	// void init(RenderContext *_ctx, const tinygltf::Model &_model, uint32_t import_flags = Flags::All);
	// void freeResources();
	// ~GLTF() { freeResources(); }

	struct Primitive {
		uint32_t node = 0;
		uint32_t mesh = 0;
		uint32_t index_offset = 0; // in uint32_t units
		uint32_t index_count = 0;
		int32_t material = -1;
		glm::vec3 bounds_min = {0.0f, 0.0f, 0.0f}; // bounds in world coordinates
		glm::vec3 bounds_max = {0.0f, 0.0f, 0.0f}; // bounds in world coordinates
	};

	struct Node {
		glm::mat4 transform; // transformation relative to its parent
		glm::mat4 model;     // model->world transform
		uint32_t parent = 0;
	};

	struct Material {
		std::string name;
		struct {
			int32_t texture = -1;
			glm::vec4 factor = {1.0f, 1.0f, 1.0f, 1.0f};
		} base_color;
		struct {
			int32_t texture = -1;
			float metallic_factor = 0.5f;
			float roughness_factor = 0.5f;
		} metallic_roughness;
		struct {
			int32_t texture = -1;
			float factor = 1.0f; // normal-scale
		} normal;
		struct {
			int32_t texture = -1;
			float factor = 1.0f; // occlusion-strength
		} occlusion;
		struct {
			int32_t texture = -1;
			glm::vec3 factor = {1.0f, 1.0f, 1.0f};
		} emmisive;
	};

	struct Texture {
		std::string uri;
		// px_render::Texture::Info info;
		// px_render::Texture tex;
	};

	// RenderContext *ctx = nullptr;
	// Buffer vertex_buffer;
	// Buffer index_buffer;
	uint32_t num_primitives = 0;
	uint32_t num_nodes = 0;
	uint32_t num_materials = 0;
	uint32_t num_textures = 0;
	std::unique_ptr<Node[]> nodes;
	std::unique_ptr<Primitive[]> primitives;
	std::unique_ptr<Texture[]> textures;
	std::unique_ptr<Material[]> materials;
	glm::vec3 bounds_min{0.0f, 0.0f, 0.0f}; // bounds in world coordinates
	glm::vec3 bounds_max{0.0f, 0.0f, 0.0f}; // bounds in world coordinates
};

} // rn