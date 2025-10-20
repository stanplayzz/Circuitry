#pragma once
#include "nodes/node.hpp"

#include <exception>

class Producer : public Node {
public:
	Producer(sf::Texture& texture, NodeManager& nodeManager, int iPorts = 0, int oPorts = 1) 
		: Node(texture, nodeManager, iPorts, oPorts) {

	}
	void update(sf::Time deltaTime) {
		if (inWorld) {
			progress += deltaTime;
			if (progress.asSeconds() >= 1) {
				progress -= sf::seconds(1);
				if (quantity >= capacity || quantity + generationRate > capacity) {
					quantity += capacity - quantity;
					quantityText.setFillColor(sf::Color::Red);
				}
				else {
					quantity += generationRate;
					quantityText.setFillColor(sf::Color(200, 200, 200));
				}
				quantityText.setString(std::to_string(quantity));
				auto bounds = quantityText.getLocalBounds();
				quantityText.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
				for (auto& connection : nodeManager.connections) {
					if (connection.fromNode == this) {
						auto amount = outputRate;
						if (quantity < outputRate && quantity >= 1) {
							amount = quantity;
						}
						if (connection.toNode->recieve(resource, amount)) {
							quantity -= amount;
							quantityText.setString(std::to_string(quantity));
						}

						quantityText.setFillColor(sf::Color(200, 200, 200));
					}
				}
			}
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