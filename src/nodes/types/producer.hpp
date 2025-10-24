#pragma once
#include "nodes/node.hpp"

#include <exception>

enum struct NodeType;

class Producer : public Node {
public:
	Producer(NodeManager& nodeManager, Resource resource, int iPorts = 0, int oPorts = 1)
		: Node(nodeManager, iPorts, oPorts), resource(resource) {

		if (!backgroundTexture.loadFromFile(ASSETS_DIR + std::string("/textures/ProducerBG.png"))) {
			throw std::runtime_error("Failed to load texture");
		}
		background.setTexture(&backgroundTexture);
		titleBackground.setFillColor(sf::Color(16, 101, 137));
	}
	void update(sf::Time deltaTime) {
		if (inWorld) {
			if (!dragging) {
				progress += deltaTime;
				if (progress.asSeconds() >= 1) {
					progress -= sf::seconds(1);
					if (quantity >= capacity || quantity + generationRate > capacity) {
						quantity += capacity - quantity;
					}
					else {
						quantity += generationRate;
					}
					for (auto& connection : nodeManager.connections) {
						if (connection.fromNode == this) {
							auto amount = outputRate;
							if (quantity < outputRate && quantity >= 1) {
								amount = quantity;
							}
							if (connection.toNode->recieve(resource, amount)) {
								quantity -= amount;
							}
						}
					}
				}
			}
		}
		else {
			quantity = 0;
		}
	}

	bool recieve(Resource& resource, int amount) override {
		return false;
	}

protected:
	int generationRate = 1;
	int outputRate = 1;
	int capacity = 0;
	Resource resource;
private:
	sf::Time progress;
	int quantity = 0;
};