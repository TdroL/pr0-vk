#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <set>
#include <stdexcept>
#include <string_view>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "../util/either.hpp"
// #include "pipeline.hpp"
#include "types.hpp"

namespace rn {

constexpr size_t numberOfComponents(DataFormat format) {
	switch (format) {
		default:
		case DataFormat::Undefined: return 0;
		case DataFormat::R8SInt: return 1;
		case DataFormat::R8SNorm: return 1;
		case DataFormat::R8UInt: return 1;
		case DataFormat::R8UNorm: return 1;
		case DataFormat::R8G8SInt: return 2;
		case DataFormat::R8G8SNorm: return 2;
		case DataFormat::R8G8UInt: return 2;
		case DataFormat::R8G8UNorm: return 2;
		case DataFormat::R8G8B8A8SInt: return 4;
		case DataFormat::R8G8B8A8SNorm: return 4;
		case DataFormat::R8G8B8A8UInt: return 4;
		case DataFormat::R8G8B8A8UNorm: return 4;
		case DataFormat::R16Float: return 1;
		case DataFormat::R16SInt: return 1;
		case DataFormat::R16SNorm: return 1;
		case DataFormat::R16UInt: return 1;
		case DataFormat::R16UNorm: return 1;
		case DataFormat::R16G16Float: return 2;
		case DataFormat::R16G16SInt: return 2;
		case DataFormat::R16G16SNorm: return 2;
		case DataFormat::R16G16UInt: return 2;
		case DataFormat::R16G16UNorm: return 2;
		case DataFormat::R16G16B16A16Float: return 4;
		case DataFormat::R16G16B16A16SInt: return 4;
		case DataFormat::R16G16B16A16SNorm: return 4;
		case DataFormat::R16G16B16A16UInt: return 4;
		case DataFormat::R16G16B16A16UNorm: return 4;
		case DataFormat::R32Float: return 1;
		case DataFormat::R32SInt: return 1;
		case DataFormat::R32UInt: return 1;
		case DataFormat::R32G32Float: return 2;
		case DataFormat::R32G32SInt: return 2;
		case DataFormat::R32G32UInt: return 2;
		case DataFormat::R32G32B32Float: return 3;
		case DataFormat::R32G32B32SInt: return 3;
		case DataFormat::R32G32B32UInt: return 3;
		case DataFormat::R32G32B32A32Float: return 4;
		case DataFormat::R32G32B32A32SInt: return 4;
		case DataFormat::R32G32B32A32UInt: return 4;
	}
}

template<class NameType>
struct BufferLayoutAttribute {
	NameType name;
	DataFormat format;
};

template<class NameType>
class BufferLayout {
public:
	struct Entry {
		NameType name;
		DataFormat format;
		size_t size;
		size_t offset;
	};

	std::vector<Entry> entries;
	size_t stride;

	explicit BufferLayout(const std::vector<BufferLayoutAttribute<NameType>> &attributes) {
		entries.reserve(static_cast<size_t>(std::end(attributes) - std::begin(attributes)));

		stride = 0;
		for (const auto &attribute : attributes) {
			size_t size = numberOfComponents(attribute.format);
			entries.push_back({attribute.name, attribute.format, size, stride});
			stride += size;
		}
	}

	explicit BufferLayout(std::initializer_list<BufferLayoutAttribute<NameType>> attributes) {
		entries.reserve(static_cast<size_t>(std::end(attributes) - std::begin(attributes)));

		stride = 0;
		for (const auto &attribute : attributes) {
			size_t size = numberOfComponents(attribute.format);
			entries.push_back({attribute.name, attribute.format, size, stride});
			stride += size;
		}
	}
};

enum class BufferUsage {
	None = 0x00000000,
	TransferSource = 0x00000001,
	TransferDestination = 0x00000002,
	Index = 0x00000004,
	Vertex = 0x00000008,
	Uniform = 0x00000010,
	Storage = 0x00000020,
};
static_assert(sizeof(BufferUsage) == sizeof(uint32_t));

constexpr BufferUsage operator|(BufferUsage left, BufferUsage right) {
	return static_cast<BufferUsage>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

constexpr BufferUsage operator&(BufferUsage left, BufferUsage right) {
	return static_cast<BufferUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

enum class TextureUsage {
	None = 0x00000000,
	TransferSource = 0x00000001,
	TransferDestination = 0x00000002,
	Sampled = 0x00000004,
	Storage = 0x00000008,
	ColorAttachment = 0x00000010,
	DepthStencilAttachment = 0x00000020,
	TransientAttachment = 0x00000040,
	InputAttachment = 0x00000080,
};
static_assert(sizeof(TextureUsage) == sizeof(uint32_t));

constexpr TextureUsage operator|(TextureUsage left, TextureUsage right) {
	return static_cast<TextureUsage>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

constexpr TextureUsage operator&(TextureUsage left, TextureUsage right) {
	return static_cast<TextureUsage>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

using BufferHandle = size_t;
using TextureHandle = size_t;
using SubpassHandle = size_t;

enum class Access {
	None = 0x00000000,
	Read = 0x00000001,
	Write = 0x00000002,
	Modify = 0x00000004,
};
static_assert(sizeof(Access) == sizeof(uint32_t));

constexpr Access operator|(Access left, Access right) {
	return static_cast<Access>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
}

constexpr Access operator&(Access left, Access right) {
	return static_cast<Access>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
}

struct TextureCreate {
	rn::PixelFormat format;
	std::optional<std::array<uint32_t, 3>> dimensions{};
	rn::ImageType type{rn::ImageType::Tex2D};
	int levels{1};
	TextureUsage usage{TextureUsage::None};
};

struct TextureModify {
	std::string source;
};

struct BufferCreate {
	size_t size;
	BufferUsage usage;
};

struct BufferModify {
	std::string source;
};

template<class Handle, class CreateDescriptor, class ModifyDescriptor>
class Descritors {
public:
	std::vector<std::pair<std::string, std::variant<std::monostate, CreateDescriptor, ModifyDescriptor>>> descriptors{};

	Handle read(std::string_view name) {
		return upsert(name, std::monostate{});
	}

	Handle create(std::string_view name, CreateDescriptor &&descriptor) {
		return upsert(name, std::move(descriptor));
	}

	Handle modify(std::string_view name, ModifyDescriptor &&descriptor) {
		return upsert(name, std::move(descriptor));
	}

private:
	template<class Descriptor>
	Handle upsert(std::string_view name, Descriptor &&descriptor) {
		auto it = std::find_if(std::begin(descriptors), std::end(descriptors), [&] (const auto &entry) {
			return entry.first == name;
		});

		if (it == std::end(descriptors)) {
			size_t index = descriptors.size();
			if constexpr (std::is_empty<Descriptor>::value) {
				descriptors.emplace_back(std::string{name}, std::monostate{});
			} else {
				descriptors.emplace_back(std::string{name}, std::move(descriptor));
			}

			return index;
		} else {
			if constexpr (std::is_empty<Descriptor>::value) {
				it->second = std::monostate{};
			} else {
				it->second = std::move(descriptor);
			}

			return it - std::begin(descriptors);
		}
	}
};

using BufferDescritors = Descritors<BufferHandle, BufferCreate, BufferModify>;
using TextureDescritors = Descritors<TextureHandle, TextureCreate, TextureModify>;

struct Resources {
	BufferDescritors buffer{};
	TextureDescritors texture{};
// public:
	// std::vector<std::pair<std::string, std::variant<std::monostate, BufferCreate, BufferModify>>> buffers{};
	// std::vector<std::pair<std::string, std::variant<std::monostate, TextureCreate, TextureModify>>> textures{};

	// template<class T>
	// BufferHandle buffer(std::string &&name, T &&value) {
	// 	auto it = std::find_if(std::begin(buffers), std::end(buffers), [&] (const auto &entry) {
	// 		return entry.first == name;
	// 	});

	// 	if (it == std::end(buffers)) {
	// 		size_t index = buffers.size();
	// 		if constexpr (std::is_empty<T>::value) {
	// 			buffers.emplace_back(std::move(name), std::monostate{});
	// 		} else {
	// 			buffers.emplace_back(std::move(name), std::move(value));
	// 		}

	// 		return index;
	// 	} else {
	// 		if constexpr (std::is_empty<T>::value) {
	// 			it->second = std::monostate{};
	// 		} else {
	// 			it->second = std::move(value);
	// 		}

	// 		return it - std::begin(buffers);
	// 	}
	// }

	// template<class T>
	// TextureHandle texture(std::string &&name, T &&value) {
	// 	auto it = std::find_if(std::begin(textures), std::end(textures), [&] (const auto &entry) {
	// 		return entry.first == name;
	// 	});

	// 	if (it == std::end(textures)) {
	// 		size_t index = textures.size();
	// 		if constexpr (std::is_empty<T>::value) {
	// 			textures.emplace_back(std::move(name), std::monostate{});
	// 		} else {
	// 			textures.emplace_back(std::move(name), std::move(value));
	// 		}

	// 		return index;
	// 	} else {
	// 		if constexpr (std::is_empty<T>::value) {
	// 			it->second = std::monostate{};
	// 		} else {
	// 			it->second = std::move(value);
	// 		}

	// 		return it - std::begin(textures);
	// 	}
	// }
};

struct Dimensions {
	uint32_t width;
	uint32_t height;
	uint32_t depth{1};
};

// enum class PassType {
// 	Graphic,
// 	Compute,
// };

// class Pass {
// public:
// 	const PassType type;
// 	const std::string name;
// 	const Dimensions dimensions;
// 	const std::function<std::pair<Resources, Subpasses> ()> setup;

// 	explicit Pass(PassType type, std::string &&name, Dimensions &&dimensions, std::function<std::pair<Resources, Subpasses> ()> &&setup) noexcept :
// 		type{type},
// 		name{std::move(name)},
// 		dimensions{std::move(dimensions)},
// 		setup{std::move(setup)}
// 	{}
// };

// class GraphicPass : public Pass {
// public:
// 	explicit GraphicPass(std::string &&name, Dimensions &&dimensions, std::function<std::pair<Resources, Subpasses> ()> &&setup) noexcept :
// 		Pass{PassType::Graphic, std::move(name), std::move(dimensions), std::move(setup)}
// 	{}
// };

// class ComputePass : public Pass {
// public:
// 	explicit ComputePass(std::string &&name, std::function<std::pair<Resources, Subpasses> ()> &&setup) noexcept :
// 		Pass{PassType::Compute, std::move(name), {0, 0, 0}, std::move(setup)}
// 	{}
// };

template<class T>
class Subpasses {
public:
	struct Entry {
		std::vector<SubpassHandle> dependencies;
		T desc;
	};

	std::vector<std::pair<std::string, Entry>> entries{};

	void reserve(size_t size) {
		entries.reserve(size);
	}

	SubpassHandle add(std::string &&name, std::vector<SubpassHandle> &&dependencies, T &&desc) {
		auto it = std::find_if(std::begin(entries), std::end(entries), [&] (const auto &entry) {
			return entry.first == name;
		});

		if (it == std::end(entries)) {
			size_t index = entries.size();
			entries.emplace_back(std::move(name), Entry{ std::move(dependencies), std::move(desc) });

			return index;
		} else {
			it->second.dependencies = std::move(dependencies);
			it->second.desc = std::move(desc);

			return it - std::begin(entries);
		}
	}
};

struct GraphicSubpassDesc {
	struct {
		TextureHandle texture{};
		Access usage{Access::None};
	} depthStencil{};
	std::vector<TextureHandle> inputs{};
	std::vector<TextureHandle> outputs{};
	std::vector<TextureHandle> textures{};
	std::vector<BufferHandle> buffers{};
};

struct ComputeSubpassDesc {
	std::vector<TextureHandle> textures{};
	std::vector<BufferHandle> buffers{};
};

using GraphicSubpasses = Subpasses<GraphicSubpassDesc>;
using ComputeSubpasses = Subpasses<ComputeSubpassDesc>;

struct GraphicPass {
	std::string name;
	Dimensions dimensions;
	std::function<std::tuple<Resources, GraphicSubpasses> ()> setup;
};

struct ComputePass {
	std::string name;
	std::function<std::tuple<Resources, ComputeSubpasses> ()> setup;
};

struct TransferPass {
	std::string name;
	std::function<Resources ()> setup;
};

// struct PassMapping {
// 	bool temporary{false};
// 	bool permanent{false};

// 	struct {
// 		std::set<std::string> creates{};
// 		std::set<std::string> reads{};
// 		std::set<std::string> modifies{};
// 	} textures{};

// 	struct {
// 		std::set<std::string> creates{};
// 		std::set<std::string> reads{};
// 		std::set<std::string> modifies{};
// 	} buffers{};
// };

// template<class T>
// struct ResourceMapping {
// 	std::optional<std::string> creator{std::nullopt};
// 	std::set<std::string> readers{};
// 	std::optional<std::string> source{std::nullopt};
// 	std::optional<T> desc{std::nullopt};
// };

class GraphCompileError {
public:
	std::string message;

	explicit GraphCompileError(GraphCompileError &&other) noexcept :
		message{std::move(other.message)}
	{}

	explicit GraphCompileError(std::string &&message) noexcept :
		message{std::move(message)}
	{}

	GraphCompileError & operator=(GraphCompileError &&other) {
		message = std::move(other.message);

		return *this;
	}
};

using CompileResult = util::EitherOption<GraphCompileError, std::vector<std::string>>;

// using BufferResourceMapping = ResourceMapping<BufferCreate>;
// using TextureResourceMapping = ResourceMapping<TextureCreate>;

class Graph {
public:
	std::vector<std::variant<GraphicPass, ComputePass, TransferPass>> passes{};

	Graph() = default;

	template<class T>
	void add(T &&pass) {
		passes.push_back(std::move(pass));
	}

	CompileResult compile([[maybe_unused]] std::string &&rootPass) {
		std::unordered_map<std::string, BufferResourceMapping> buffers{};
		std::unordered_map<std::string, TextureResourceMapping> textures{};
		std::unordered_map<std::string, PassMapping> passMapping{};

		std::vector<Resources> resourceList{};
		std::vector<Subpasses> subpassesList{};

		for (const auto &pass : passes) {
			auto [resources, subpasses] = pass.setup();

			if (passMapping.find(pass.name) == std::end(passMapping)) {
				passMapping.insert_or_assign(pass.name, PassMapping{});
			}

			for (auto it : resources.textures) {
				const std::string key{it.first};
				if (textures.find(key) == std::end(textures)) {
					textures.insert_or_assign(key, TextureResourceMapping{});
				}

				auto &texture = textures[key];

				std::visit(util::fold{
					[&] (const TextureCreate &value) {
						texture.creator = pass.name;
						texture.source = std::nullopt;
						texture.desc = static_cast<TextureCreate>(value);

						passMapping[pass.name].textures.creates.insert(key);
					},
					[&] (const TextureModify &value) {
						texture.creator = pass.name;
						texture.source = value.source;
						texture.desc = std::nullopt;

						passMapping[pass.name].textures.modifies.insert(key);
					},
					[&] (const TextureRead &) {
						texture.readers.insert(pass.name);

						passMapping[pass.name].textures.reads.insert(key);
					}
				}, it.second);
			}

			for (auto it : resources.buffers) {
				const std::string key{it.first};
				if (buffers.find(key) == std::end(buffers)) {
					buffers.insert_or_assign(key, BufferResourceMapping{});
				}

				auto &buffer = buffers[key];

				std::visit(util::fold{
					[&] (const BufferCreate &value) {
						buffer.creator = pass.name;
						buffer.source = std::nullopt;
						buffer.desc = static_cast<BufferCreate>(value);

						passMapping[pass.name].buffers.creates.insert(key);
					},
					[&] (const BufferModify &value) {
						buffer.creator = pass.name;
						buffer.source = value.source;
						buffer.desc = std::nullopt;

						passMapping[pass.name].buffers.modifies.insert(key);
					},
					[&] (const BufferRead &) {
						buffer.readers.insert(pass.name);

						passMapping[pass.name].buffers.reads.insert(key);
					}
				}, it.second);
			}

			resourceList.push_back(std::move(resources));
			subpassesList.push_back(std::move(subpasses));
		}

		return dfs(rootPass, passMapping, buffers, textures);
	}

	void sortPasses(const std::unordered_map<std::string, PassMapping> &passMapping, std::vector<std::string> &dependencies) {
		std::sort(std::begin(dependencies), std::end(dependencies), [&passMapping] (const std::string &aName, const std::string &bName) {
			const auto aIt = passMapping.find(aName);
			const auto bIt = passMapping.find(bName);

			if (aIt == std::end(passMapping) || bIt == std::end(passMapping)) {
				return false;
			}

			const auto &a = aIt->second;
			const auto &b = bIt->second;

			size_t aComplexity = a.textures.reads.size() + a.textures.modifies.size() + a.buffers.reads.size() + a.buffers.modifies.size();
			size_t bComplexity = b.textures.reads.size() + b.textures.modifies.size() + b.buffers.reads.size() + b.buffers.modifies.size();

			if (aComplexity == bComplexity) {
				aComplexity = a.textures.creates.size() + a.buffers.creates.size();
				bComplexity = b.textures.creates.size() + b.buffers.creates.size();
			}

			return aComplexity < bComplexity;
		});
	};

	CompileResult dfs(const std::string &passName, std::unordered_map<std::string, PassMapping> &passMapping, const std::unordered_map<std::string, BufferResourceMapping> &buffers, const std::unordered_map<std::string, TextureResourceMapping> &textures, std::vector<std::string> &&list = {}) {
		const auto passIt = passMapping.find(passName);
		if (passIt == std::end(passMapping)) {
			return GraphCompileError{"Pass \"" + passName + "\" not found"};
		}

		auto &mapping = passIt->second;

		if (mapping.permanent) {
			return std::move(list);
		}

		if (mapping.temporary) {
			return GraphCompileError{"Cyclic dependency found on pass \"" + passName + "\""};
		}

		mapping.temporary = true;

		std::vector<std::string> dependencies{};

		for (const std::string &bufName : mapping.buffers.modifies) {
			const auto bufIt = buffers.find(bufName);
			if (bufIt == std::end(buffers)) {
				return GraphCompileError{"Buffer \"" + bufName + "\" not found [pass: \"" + passName + "\"]"};
			}

			if ( ! bufIt->second.source) {
				return GraphCompileError{"Buffer \"" + bufName + "\" has no source [pass: \"" + passName + "\"]"};
			}

			const auto &sourceName = bufIt->second.source.value();

			const auto sourceIt = buffers.find(sourceName);
			if (sourceIt == std::end(buffers)) {
				return GraphCompileError{"Source buffer \"" + sourceName + "\" not found [pass: \"" + passName + "\"]"};
			}

			for (const auto &subpassName : sourceIt->second.readers) {
				dependencies.push_back(subpassName);
			}

			if ( ! sourceIt->second.creator) {
				return GraphCompileError{"Buffer \"" + sourceName + "\" has no creator [pass: \"" + passName + "\"]"};
			}

			dependencies.push_back(sourceIt->second.creator.value());
		}

		for (const std::string &texName : mapping.textures.modifies) {
			const auto texIt = textures.find(texName);
			if (texIt == std::end(textures)) {
				return GraphCompileError{"Texture \"" + texName + "\" not found [pass: \"" + passName + "\"]"};
			}

			if ( ! texIt->second.source) {
				return GraphCompileError{"Texture \"" + texName + "\" has no source [pass: \"" + passName + "\"]"};
			}

			const auto &sourceName = texIt->second.source.value();

			const auto sourceIt = textures.find(sourceName);
			if (sourceIt == std::end(textures)) {
				return GraphCompileError{"Source texture \"" + sourceName + "\" not found [pass: \"" + passName + "\"]"};
			}

			for (const auto &subpassName : sourceIt->second.readers) {
				dependencies.push_back(subpassName);
			}

			if ( ! sourceIt->second.creator) {
				return GraphCompileError{"Texture \"" + sourceName + "\" has no creator [pass: \"" + passName + "\"]"};
			}

			dependencies.push_back(sourceIt->second.creator.value());
		}

		for (const auto &bufName : mapping.buffers.reads) {
			const auto bufIt = buffers.find(bufName);
			if (bufIt == std::end(buffers)) {
				return GraphCompileError{"Buffer \"" + bufName + "\" not found [pass: \"" + passName + "\"]"};
			}

			if ( ! bufIt->second.creator) {
				return GraphCompileError{"Buffer \"" + bufName + "\" has no creator [pass: \"" + passName + "\"]"};
			}

			dependencies.push_back(bufIt->second.creator.value());
		}

		for (const auto &texName : mapping.textures.reads) {
			const auto texIt = textures.find(texName);
			if (texIt == std::end(textures)) {
				return GraphCompileError{"Texture \"" + texName + "\" not found [pass: \"" + passName + "\"]"};
			}

			if ( ! texIt->second.creator) {
				return GraphCompileError{"Texture \"" + texName + "\" has no creator [pass: \"" + passName + "\"]"};
			}

			dependencies.push_back(texIt->second.creator.value());
		}

		dependencies.erase(std::unique(std::begin(dependencies), std::end(dependencies)), std::end(dependencies));
		sortPasses(passMapping, dependencies);

		std::vector<std::string> newList{list};
		for (const auto &dependency : dependencies) {
			auto result = dfs(dependency, passMapping, buffers, textures, std::move(newList));

			if (result.isRight()) {
				newList = std::move(result.right());
			} else {
				return std::move(result.left());
			}
		}

		mapping.permanent = true;

		newList.push_back(passName);

		return newList;
	}
};

} // rn
