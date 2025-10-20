#pragma once
#include "nodes/node.hpp"
#include "resources.hpp"

#include <exception>

struct Converter : public Node {
	Resource resource = Resource::Iron;
	int quantity = 0;

	Converter(sf::Texture& texture, NodeManager& nodeManager, int iPorts = 1, int oPorts = 1) 
		: Node(texture, nodeManager, iPorts, oPorts) {

	}

	void update(sf::Time deltaTime) {
	}

	bool recieve(Resource& res, int amount) override {
		if (res != resource) return false;

		quantity += amount;
		quantityText.setString(std::to_string(quantity));

		return true;
	}
};