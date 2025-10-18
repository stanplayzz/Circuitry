#pragma once
#include "nodes/node.hpp"

#include <exception>

struct Converter : public Node {
	Converter(sf::Texture& texture, NodeManager& nodeManager, int iPorts = 1, int oPorts = 1) 
		: Node(texture, nodeManager, iPorts, oPorts) {

	}

	void update(sf::Time deltaTime) {
	}
};