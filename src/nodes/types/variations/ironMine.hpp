#pragma once

#include "nodes/types/producer.hpp"

#include <print>

class IronMine : public Producer {
public:
	IronMine(NodeManager& nodeManager) 
		: Producer(nodeManager, Resource::Iron, 0, 1) {
		if (!texture.loadFromFile(ASSETS_DIR + std::string("/textures/IronMine.png")))
			std::println("Failed to load IronMine texture");
		image.setTexture(texture);
		generationRate = 30;
		outputRate = 20;
		capacity = 500;
		titleText.setString("Iron mine");
	}
private:

};