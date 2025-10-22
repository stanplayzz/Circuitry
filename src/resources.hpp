#pragma once

#include <string>

struct Resource {
	enum Type {
		Iron,
		Coal
	} type;

	constexpr Resource(Type t) : type(t) {}

	constexpr std::string as_string() const {
		switch (type) {
		case Resource::Iron: return "Iron";
		case Resource::Coal: return "Coal";
		default: return "ErrorType";
		}
	}

	bool operator==(const Resource& other) const {
		return type == other.type;
	}

	bool operator!=(const Resource& other) const {
		return !(*this == other);
	}
};