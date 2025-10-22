#pragma once
#include "nodes/node.hpp"

#include <exception>

enum struct NodeType;

class Producer : public Node {
public:
	Producer(sf::Texture& texture, NodeManager& nodeManager, Resource resource, int iPorts = 0, int oPorts = 1)
		: Node(texture, nodeManager, iPorts, oPorts), resource(resource) {
	}
	void update(sf::Time deltaTime) {
		if (inWorld) {
			if (!dragging) {
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
					quantityText.setOrigin({ bounds.size / 2.f });
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
		else {
			quantity = 0;
			quantityText.setString("");
		}
	}

	bool recieve(Resource& resource, int amount) override {
		return false;
	}
	void setDerivedPosition(sf::Vector2f position) override {

	}
	void drawDerived(sf::RenderWindow& window) override {

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