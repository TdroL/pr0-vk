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

rn::graph::Resources applyPassProperties(const rn::graph::GraphicPass &pass, rn::graph::Resources &&resources) {
	for (auto &&it : resources.texture) {
		if (auto value = std::get_if<rn::graph::TextureCreate>(&it.second)) {
			if ( ! value->dimensions) {
				value->dimensions = {{ pass.dimensions.width, pass.dimensions.height, pass.dimensions.depth }};
			}
		}
	}

	return resources;
}

rn::graph::Resources applyPassProperties([[maybe_unused]] const rn::graph::ComputePass &pass, rn::graph::Resources &&resources) {
	return resources;
}

rn::graph::Resources applyPassProperties([[maybe_unused]] const rn::graph::TransferPass &pass, rn::graph::Resources &&resources) {
	return resources;
}

void registerResources(rn::graph::MappingMap &mapping, std::string_view passName, const rn::graph::Resources &resources) {
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
		} else {
			bufferMapping.readers.insert(passName);

			passMapping.buffers.reads.insert(key);
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
		} else {
			textureMapping.readers.insert(passName);

			passMapping.textures.reads.insert(key);
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

	std::vector<rn::graph::ResourcesUsage> resourcesUsageList{};
	resourcesUsageList.reserve(passes.size());

	for (size_t i = 0; i < passes.size(); i++) {
		auto &pass = passes[i];

		if (auto value = std::get_if<rn::graph::GraphicPass>(&pass)) {
			auto [resources, subpasses, recorders] = value->setup();
			resourcesUsageList.push_back(rn::graph::ResourcesUsage{std::string{value->name}, applyPassProperties(*value, std::move(resources))});

			registerResources(mapping, resourcesUsageList.back().passName, resourcesUsageList.back().resources);
		} else if (auto value = std::get_if<rn::graph::ComputePass>(&pass)) {
			auto [resources, subpasses, recorders] = value->setup();
			resourcesUsageList.push_back(rn::graph::ResourcesUsage{std::string{value->name}, applyPassProperties(*value, std::move(resources))});

			registerResources(mapping, resourcesUsageList.back().passName, resourcesUsageList.back().resources);
		} else if (auto value = std::get_if<rn::graph::TransferPass>(&pass)) {
			auto [resources, recorder] = value->setup();
			resourcesUsageList.push_back(rn::graph::ResourcesUsage{std::string{value->name}, applyPassProperties(*value, std::move(resources))});

			registerResources(mapping, resourcesUsageList.back().passName, resourcesUsageList.back().resources);
		} else {
			ngn::log::warn("rn::graph::compile([passes: {}], {}) => unknown pass type for item {} (index: {})", passes.size(), std::string{rootPass}, pass.index(), i);
			continue;
		}
	}

	auto dependentPassesE = findDependentPasses(mapping, rootPass);

	// if (dependentPassesE.isRight()) {
	// 	ngn::log::debug("dependentPasses={}", util::join(util::map(dependentPassesE.right(), [] (const auto &item) {
	// 		return util::format("{}[{}]", std::get<0>(item), std::get<1>(item));
	// 		// return std::string{std::get<0>(item)} + "[" + std::string{std::get<1>(item)} + "]";
	// 	})));
	// } else {
	// 	ngn::log::debug("error={}", dependentPassesE.left().message);
	// }

	// for (auto &&buffer : mapping.buffers) {
	// 	const auto desc = buffer.second.desc.value_or(BufferCreate{});
	// 	std::vector<std::string> readers{std::begin(buffer.second.readers), std::end(buffer.second.readers)};
	// 	ngn::log::debug("buffer={} ({}):\n  .creator={}\n  .size={}\n  .usage={}\n  .readers={}", buffer.first, reinterpret_cast<const void *>(buffer.first.data()), buffer.second.creator.value_or("[null]"), desc.size, static_cast<uint32_t>(desc.usage), util::join(readers));
	// }

	// for (auto &&texture : mapping.textures) {
	// 	const auto desc = texture.second.desc.value_or(TextureCreate{});
	// 	std::vector<std::string> readers{std::begin(texture.second.readers), std::end(texture.second.readers)};
	// 	ngn::log::debug("texture={} ({}):\n  .creator={}\n  .format={}\n  .type={}\n  .readers={}", texture.first, reinterpret_cast<const void *>(texture.first.data()), texture.second.creator.value_or("[null]"), static_cast<uint32_t>(desc.format), static_cast<uint32_t>(desc.type), util::join(readers));
	// }

	// for (auto &&mapping : mapping.passes) {
	// 	std::string bufferCreates{util::join(std::vector<std::string>{std::begin(mapping.second.buffers.creates), std::end(mapping.second.buffers.creates)})};
	// 	std::string bufferReads{util::join(std::vector<std::string>{std::begin(mapping.second.buffers.reads), std::end(mapping.second.buffers.reads)})};
	// 	std::string bufferModifies{util::join(std::vector<std::string>{std::begin(mapping.second.buffers.modifies), std::end(mapping.second.buffers.modifies)})};
	// 	std::string textureCreates{util::join(std::vector<std::string>{std::begin(mapping.second.textures.creates), std::end(mapping.second.textures.creates)})};
	// 	std::string textureReads{util::join(std::vector<std::string>{std::begin(mapping.second.textures.reads), std::end(mapping.second.textures.reads)})};
	// 	std::string textureModifies{util::join(std::vector<std::string>{std::begin(mapping.second.textures.modifies), std::end(mapping.second.textures.modifies)})};

	// 	ngn::log::debug("pass={} ({}):\n  .buffer.creates={}\n  .buffer.reads={}\n  .buffer.modifies={}\n  .texture.creates={}\n  .texture.reads={}\n  .texture.modifies={}", mapping.first, reinterpret_cast<const void *>(mapping.first.data()), bufferCreates, bufferReads, bufferModifies, textureCreates, textureReads, textureModifies);
	// }

	if (dependentPassesE.isRight()) {
		std::vector<std::string_view> resolvedPassNames = util::map(dependentPassesE.right(), [&] (const auto &item) {
			return std::get<std::string_view>(item);
		});

		resourcesUsageList.erase(std::remove_if(std::begin(resourcesUsageList), std::end(resourcesUsageList), [&] (auto &resourcesUsage) {
			return ! util::contains(resolvedPassNames, std::string_view{resourcesUsage.passName});
		}), std::end(resourcesUsageList));

		return CompileData{
			/*.resolvedPassNames=*/ std::move(resolvedPassNames),
			/*.resourcesUsageList=*/ std::move(resourcesUsageList),
		};
	} else {
		return std::move(dependentPassesE.left());
	}
}

} // rn::graph
