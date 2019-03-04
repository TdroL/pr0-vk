#include "compile.hpp"

#include <unordered_set>
#include <unordered_map>
#include <variant>

#include <vulkan/vulkan.hpp>

#include "../../ngn/log.hpp"
#include "../../util/contains.hpp"
#include "../../util/format.hpp"
#include "../../util/join.hpp"
#include "../../util/map.hpp"

namespace rn::graph {

template<class T>
struct ResourceMapping {
	std::optional<std::string_view> creator{std::nullopt};
	std::unordered_set<std::string_view> readers{};
	std::optional<std::string_view> source{std::nullopt};
	std::optional<T> desc{std::nullopt};
};

using BufferResourceMapping = rn::graph::ResourceMapping<BufferCreate>;
using TextureResourceMapping = rn::graph::ResourceMapping<TextureCreate>;

struct ResourceSets {
	std::unordered_set<std::string_view> creates{};
	std::unordered_set<std::string_view> reads{};
	std::unordered_set<std::string_view> modifies{};
};

struct PassMapping {
	bool temporary{false};
	bool permanent{false};

	rn::graph::ResourceSets textures{};
	rn::graph::ResourceSets buffers{};
};

using BufferDescriptor = std::variant<std::monostate, rn::graph::BufferCreate, rn::graph::BufferModify>;
using TextureDescriptor = std::variant<std::monostate, rn::graph::TextureCreate, rn::graph::TextureModify>;

struct MappingMap {
	std::unordered_map<std::string_view, rn::graph::BufferResourceMapping> buffers{};
	std::unordered_map<std::string_view, rn::graph::TextureResourceMapping> textures{};
	std::unordered_map<std::string_view, rn::graph::PassMapping> passes{};
};

rn::graph::ResourceDescriptors applyPassProperties(const rn::graph::GraphicPass &pass, rn::graph::ResourceDescriptors &&resources) {
	for (auto &&it : resources.texture) {
		if (auto value = std::get_if<rn::graph::TextureCreate>(&it.second)) {
			if (value->dimensions.width == 0) {
				value->dimensions.width = pass.dimensions.width;
			}

			if (value->dimensions.height == 0) {
				value->dimensions.height = pass.dimensions.height;
			}

			if (value->dimensions.depth == 0) {
				value->dimensions.depth = pass.dimensions.depth;
			}

			value->dimensions.width = std::max(1u, pass.dimensions.width);
			value->dimensions.height = std::max(1u, pass.dimensions.height);
			value->dimensions.depth = std::max(1u, pass.dimensions.depth);
		}
	}

	return resources;
}

rn::graph::ResourceDescriptors applyPassProperties([[maybe_unused]] const rn::graph::ComputePass &pass, rn::graph::ResourceDescriptors &&resources) {
	return resources;
}

rn::graph::ResourceDescriptors applyPassProperties([[maybe_unused]] const rn::graph::TransferPass &pass, rn::graph::ResourceDescriptors &&resources) {
	return resources;
}

void registerResources(rn::graph::MappingMap &mapping, std::string_view passName, const rn::graph::ResourceDescriptors &resources) {
	const auto passIt = mapping.passes.try_emplace(passName, rn::graph::PassMapping{});
	auto &passMapping = passIt.first->second;

	for (auto &&it : resources.buffer) {
		const std::string_view key = it.first;

		const auto bufferIt = mapping.buffers.try_emplace(key, rn::graph::BufferResourceMapping{});
		auto &bufferMapping = bufferIt.first->second;

		if (auto value = std::get_if<rn::graph::BufferCreate>(&it.second)) {
			bufferMapping.creator = passName;
			bufferMapping.source = std::nullopt;
			bufferMapping.desc = *value;

			passMapping.buffers.creates.insert(key);
		} else if (auto value = std::get_if<rn::graph::BufferModify>(&it.second)) {
			bufferMapping.creator = passName;
			bufferMapping.source = value->source;
			bufferMapping.desc = std::nullopt;

			passMapping.buffers.modifies.insert(key);
		} else if (std::holds_alternative<std::monostate>(it.second)) {
			bufferMapping.readers.insert(passName);

			passMapping.buffers.reads.insert(key);
		} else {
			ngn::log::warn("rn::graph::registerResource({}) => unknown resource type for buffer {}", passName, it.first);
		}
	}

	for (auto &&it : resources.texture) {
		const std::string_view key = it.first;

		const auto textureIt = mapping.textures.try_emplace(key, rn::graph::TextureResourceMapping{});
		auto &textureMapping = textureIt.first->second;

		if (auto value = std::get_if<rn::graph::TextureCreate>(&it.second)) {
			textureMapping.creator = passName;
			textureMapping.source = std::nullopt;
			textureMapping.desc = *value;

			passMapping.textures.creates.insert(key);
		} else if (auto value = std::get_if<rn::graph::TextureModify>(&it.second)) {
			textureMapping.creator = passName;
			textureMapping.source = value->source;
			textureMapping.desc = std::nullopt;

			passMapping.textures.modifies.insert(key);
		} else if (std::holds_alternative<std::monostate>(it.second)) {
			textureMapping.readers.insert(passName);

			passMapping.textures.reads.insert(key);
		} else {
			ngn::log::warn("rn::graph::registerResource({}) => unknown resource type for texture {}", passName, it.first);
		}
	}
}

using DependenciesResult = util::EitherOption<rn::graph::CompileError, std::vector<std::string_view>>;
using ResolveResult = util::EitherOption<rn::graph::CompileError, std::vector<std::tuple<std::string_view, uint32_t>>>;

rn::graph::DependenciesResult findDependencies(rn::graph::MappingMap &mapping, std::string_view passName) {
	const auto passIt = mapping.passes.find(passName);
	if (passIt == std::end(mapping.passes)) {
		return rn::graph::CompileError{"Unknown pass \"" + std::string{passName} + "\""};
	}

	auto &passMapping = passIt->second;

	std::vector<std::string_view> dependencies{};

	for (auto &&bufferName : passMapping.buffers.modifies) {
		const auto bufferIt = mapping.buffers.find(bufferName);
		if (bufferIt == std::end(mapping.buffers)) {
			return rn::graph::CompileError{"Buffer \"" + std::string{bufferName} + "\" not found [pass: \"" + std::string{passName} + "\"]"};
		}

		if ( ! bufferIt->second.source) {
			return rn::graph::CompileError{"Buffer \"" + std::string{bufferName} + "\" has no source [pass: \"" + std::string{passName} + "\"]"};
		}

		const auto &sourceName = bufferIt->second.source.value();

		const auto sourceIt = mapping.buffers.find(sourceName);
		if (sourceIt == std::end(mapping.buffers)) {
			return rn::graph::CompileError{"Source buffer \"" + std::string{sourceName} + "\" not found [pass: \"" + std::string{passName} + "\"]"};
		}

		for (auto &&subpassName : sourceIt->second.readers) {
			dependencies.push_back(subpassName);
		}

		if ( ! sourceIt->second.creator) {
			return rn::graph::CompileError{"Buffer \"" + std::string{sourceName} + "\" has no creator [pass: \"" + std::string{passName} + "\"]"};
		}

		dependencies.push_back(sourceIt->second.creator.value());
	}

	for (auto &&bufferName : passMapping.buffers.reads) {
		const auto bufferIt = mapping.buffers.find(bufferName);
		if (bufferIt == std::end(mapping.buffers)) {
			return rn::graph::CompileError{"Buffer \"" + std::string{bufferName} + "\" not found [pass: \"" + std::string{passName} + "\"]"};
		}

		if ( ! bufferIt->second.creator) {
			return rn::graph::CompileError{"Buffer \"" + std::string{bufferName} + "\" has no creator [pass: \"" + std::string{passName} + "\"]"};
		}

		dependencies.push_back(bufferIt->second.creator.value());
	}

	for (auto &&textureName : passMapping.textures.modifies) {
		const auto textureIt = mapping.textures.find(textureName);
		if (textureIt == std::end(mapping.textures)) {
			return rn::graph::CompileError{"Texture \"" + std::string{textureName} + "\" not found [pass: \"" + std::string{passName} + "\"]"};
		}

		if ( ! textureIt->second.source) {
			return rn::graph::CompileError{"Texture \"" + std::string{textureName} + "\" has no source [pass: \"" + std::string{passName} + "\"]"};
		}

		const auto &sourceName = textureIt->second.source.value();

		const auto sourceIt = mapping.textures.find(sourceName);
		if (sourceIt == std::end(mapping.textures)) {
			return rn::graph::CompileError{"Source texture \"" + std::string{sourceName} + "\" not found [pass: \"" + std::string{passName} + "\"]"};
		}

		for (auto &&subpassName : sourceIt->second.readers) {
			dependencies.push_back(subpassName);
		}

		if ( ! sourceIt->second.creator) {
			return rn::graph::CompileError{"Texture \"" + std::string{sourceName} + "\" has no creator [pass: \"" + std::string{passName} + "\"]"};
		}

		dependencies.push_back(sourceIt->second.creator.value());
	}

	for (auto &&textureName : passMapping.textures.reads) {
		const auto textureIt = mapping.textures.find(textureName);
		if (textureIt == std::end(mapping.textures)) {
			return rn::graph::CompileError{"Texture \"" + std::string{textureName} + "\" not found [pass: \"" + std::string{passName} + "\"]"};
		}

		if ( ! textureIt->second.creator) {
			return rn::graph::CompileError{"Texture \"" + std::string{textureName} + "\" has no creator [pass: \"" + std::string{passName} + "\"]"};
		}

		dependencies.push_back(textureIt->second.creator.value());
	}

	dependencies.erase(std::unique(std::begin(dependencies), std::end(dependencies)), std::end(dependencies));

	return dependencies;
}

void sortDependencies(const rn::graph::MappingMap &mapping, std::vector<std::string_view> &dependencies) {
	std::sort(std::begin(dependencies), std::end(dependencies), [&mapping] (const std::string_view &aName, const std::string_view &bName) {
		const auto aIt = mapping.passes.find(aName);
		const auto bIt = mapping.passes.find(bName);

		if (aIt == std::end(mapping.passes) || bIt == std::end(mapping.passes)) {
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

		return aComplexity > bComplexity;
	});
}

rn::graph::ResolveResult findDependentPasses(rn::graph::MappingMap &mapping, std::string_view passName, uint32_t level = 0) {
	const auto passIt = mapping.passes.find(passName);
	if (passIt == std::end(mapping.passes)) {
		return rn::graph::CompileError{"Unknown pass \"" + std::string{passName} + "\""};
	}

	auto &passMapping = passIt->second;

	if (passMapping.permanent) {
		return std::vector<std::tuple<std::string_view, uint32_t>>{};
	}

	if (passMapping.temporary) {
		return rn::graph::CompileError{"Cyclic dependency found while processing pass \"" + std::string{passName} + "\""};
	}

	passMapping.temporary = true;

	auto dependenciesE = findDependencies(mapping, passName);
	if (dependenciesE.isLeft()) {
		return std::move(dependenciesE.left());
	}

	std::vector<std::string_view> dependencies{std::move(dependenciesE.right())};

	sortDependencies(mapping, dependencies);

	std::vector<std::tuple<std::string_view, uint32_t>> list{};

	for (const auto &dependency : dependencies) {
		auto resultE = findDependentPasses(mapping, dependency, level + 1);

		if (resultE.isRight()) {
			list.insert(std::end(list), std::begin(resultE.right()), std::end(resultE.right()));
		} else {
			return std::move(resultE.left());
		}
	}

	list.push_back(std::make_tuple(passName, level));

	passMapping.permanent = true;

	return list;
}

rn::graph::CompileResult compile(rn::graph::Passes &&passes, std::string_view rootPass) {
	rn::graph::MappingMap mapping{};
	mapping.buffers.reserve(passes.size() * 4);
	mapping.textures.reserve(passes.size() * 4);
	mapping.passes.reserve(passes.size());

	util::FlatStorage<std::string, rn::graph::SetupResult> setupResults{};
	setupResults.reserve(passes.size());

	for (size_t i = 0; i < passes.size(); i++) {
		auto &pass = passes[i];

		if (auto value = std::get_if<rn::graph::GraphicPass>(&pass)) {
			auto setupResult = value->setup();

			setupResult.resourceDescriptors = applyPassProperties(*value, std::move(setupResult.resourceDescriptors));
			registerResources(mapping, value->name, setupResult.resourceDescriptors);

			setupResults.assign(std::string{value->name}, std::move(setupResult));
		} else if (auto value = std::get_if<rn::graph::ComputePass>(&pass)) {
			auto setupResult = value->setup();

			setupResult.resourceDescriptors = applyPassProperties(*value, std::move(setupResult.resourceDescriptors));
			registerResources(mapping, value->name, setupResult.resourceDescriptors);

			setupResults.assign(std::string{value->name}, std::move(setupResult));
		} else if (auto value = std::get_if<rn::graph::TransferPass>(&pass)) {
			auto setupResult = value->setup();

			setupResult.resourceDescriptors = applyPassProperties(*value, std::move(setupResult.resourceDescriptors));
			registerResources(mapping, value->name, setupResult.resourceDescriptors);

			setupResults.assign(std::string{value->name}, std::move(setupResult));
		} else {
			ngn::log::warn("rn::graph::compile([passes: {}], {}) => unknown pass type for item {} (index: {})", passes.size(), std::string{rootPass}, pass.index(), i);
			continue;
		}
	}

	auto dependentPassesE = findDependentPasses(mapping, rootPass);

	if (dependentPassesE.isRight()) {
		std::vector<std::string_view> resolvedPassNames = util::map(dependentPassesE.right(), [] (const auto &item) {
			return std::get<std::string_view>(item);
		});

		setupResults.entries.erase(std::remove_if(std::begin(setupResults.entries), std::end(setupResults.entries), [&] (auto &entry) {
			return ! util::contains(resolvedPassNames, std::string_view{entry.first});
		}), std::end(setupResults.entries));

		return CompileData{
			/*.resolvedPassNames=*/ std::move(resolvedPassNames),
			/*.setupResults=*/ std::move(setupResults),
		};
	} else {
		return std::move(dependentPassesE.left());
	}
}

} // rn::graph
