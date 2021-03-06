#pragma once

#include <algorithm>
#include <functional>
#include <optional>
#include <vector>

namespace util {

template<typename K, typename V>
class FlatStorage {
public:
	std::vector<std::pair<K, V>> entries{};

	using Handle = size_t;
	using Iterator = typename std::vector<std::pair<K, V>>::iterator;
	using ConstIterator = typename std::vector<std::pair<K, V>>::const_iterator;

	struct FoundResult {
		Handle handle;
		K &key;
		V &value;
	};

	void reserve(size_t size) {
		entries.reserve(size);
	}

	Handle assign(K &&key, V &&value) {
		auto it = std::find_if(std::begin(entries), std::end(entries), [&] (const auto &entry) {
			return entry.first == key;
		});

		if (it == std::end(entries)) {
			size_t index = entries.size();
			entries.emplace_back(std::move(key), std::move(value));

			return static_cast<Handle>(index);
		} else {
			it->second = std::move(value);

			return static_cast<Handle>(it - std::begin(entries));
		}
	}

	template<typename K2>
	std::optional<FoundResult> find(const K2 &key) {
		const auto it = std::find_if(std::begin(entries), std::end(entries), [&] (const auto &entry) {
			return entry.first == key;
		});

		if (it == std::end(entries)) {
			return std::nullopt;
		} else {
			return { FoundResult{ static_cast<Handle>(it - std::begin(entries)), it->first, it->second } };
		}
	}

	FoundResult findOrAssign(K &&key, V &&value = V{}) {
		auto foundOpt = find(key);

		if (foundOpt.has_value()) {
			return *foundOpt;
		} else {
			size_t index = entries.size();
			entries.emplace_back(std::move(key), std::move(value));

			return { static_cast<Handle>(index), entries[index].first, entries[index].second };
		}
	}

	std::optional<std::reference_wrapper<std::pair<K, V>>> access(const Handle &handle) noexcept {
		if (handle < entries.size()) {
			return std::ref(entries[handle]);
		} else {
			return std::nullopt;
		}
	}

	Iterator begin() {
		return entries.begin();
	}

	ConstIterator begin() const {
		return entries.begin();
	}

	ConstIterator cbegin() const {
		return entries.cbegin();
	}

	Iterator end() {
		return entries.end();
	}

	ConstIterator end() const {
		return entries.end();
	}

	ConstIterator cend() const {
		return entries.cend();
	}
};

} // util
