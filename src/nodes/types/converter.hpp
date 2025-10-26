#pragma once
#include "nodes/node.hpp"
#include "resources.hpp"
#include "recipe.hpp"

#include <exception>

struct Converter : public Node {
	Resource resource;
	int outputRate = 1;
	int capacity = 0;
	int quantity = 0;

	Converter(NodeManager& nodeManager, Resource resource, int iPorts = 1, int oPorts = 1) 
		: Node(nodeManager, iPorts, oPorts), resource(resource) {
		if (!backgroundTexture.loadFromFile(ASSETS_DIR + std::string("/textures/ConverterBG.png"))) {
			throw std::runtime_error("Failed to load texture");
		}
		background.setTexture(&backgroundTexture);
		titleBackground.setFillColor(sf::Color(127, 77, 26));
	}

	void update(sf::Time deltaTime) {
		if (inWorld) {
		}
		else {
			quantity = 0;
		}
	}

	bool recieve(Resource& res, int amount) override {
		if (res != resource) return false;
		if (dragging) return false;
		if (quantity >= capacity) return false;

		if (quantity + amount > capacity) {
			quantity = capacity;
		}
		else {
			quantity += amount;
		}

		return true;
	}
};