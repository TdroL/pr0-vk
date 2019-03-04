#pragma once

#include <vector>
#include <optional>

namespace util {

template<typename K, typename V>
class FlatStorage {
public:
	std::vector<std::pair<K, V>> entries{};

	using Handle = size_t;
	using Iterator = typename std::vector<std::pair<K, V>>::iterator;
	using ConstIterator = typename std::vector<std::pair<K, V>>::const_iterator;

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

			return index;
		} else {
			it->second = std::move(value);

			return it - std::begin(entries);
		}
	}

	std::pair<Handle, std::reference_wrapper<std::pair<K, V>>> findOrAssign(K &&key, V &&value = V{}) {
		const auto it = std::find_if(std::begin(entries), std::end(entries), [&] (const auto &entry) {
			return entry.first == key;
		});

		if (it == std::end(entries)) {
			entries.emplace_back(std::move(key), std::move(value));

			return std::make_pair(entries.size(), std::ref(entries[entries.size()]));
		} else {
			return std::make_pair(it - std::begin(entries), std::ref(*it));
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
