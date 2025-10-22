#pragma once
#include "nodes/node.hpp"
#include "resources.hpp"
#include "recipe.hpp"
#include "../nodeConfig.hpp"

#include <exception>

enum struct NodeType;

struct Converter : public Node {
	Resource resource;
	int outputRate = 1;
	int capacity = 0;
	int quantity = 0;
	sf::RectangleShape configureButton{ {100.f, 30.f } };
	sf::Text configureText;
	NodeConfig nodeConfig;
	Recipe rec{resource, resource, 10, 5};

	Converter(sf::Texture& texture, NodeManager& nodeManager, Resource resource, int iPorts = 1, int oPorts = 1) 
		: Node(texture, nodeManager, iPorts, oPorts), resource(resource), configureText(font), nodeConfig(font) {
		configureButton.setFillColor(sf::Color(100, 100, 250));
		configureText.setCharacterSize(15.f);
		setDerivedPosition({});

		nodeConfig.title.setString("Choose A Recipe");
		nodeConfig.title.setCharacterSize(15);

		rec.setSize({50.f, 50.f});
	}

	void update(sf::Time deltaTime) {
		if (inWorld) {
			if (quantity >= capacity) {
				quantityText.setFillColor(sf::Color::Red);
			}
			else {
				quantityText.setFillColor(sf::Color(200, 200, 200));
			}
		}
		else {
			quantity = 0;
			quantityText.setString("");
		}
	}

	void setDerivedPosition(sf::Vector2f position) override {
		configureButton.setPosition(position + sf::Vector2f( size.x / 2.f - configureButton.getSize().x / 2.f, size.y - configureButton.getSize().y - 10.f));

		configureText.setString("Configure");
		auto bounds = configureText.getLocalBounds();
		configureText.setOrigin(bounds.size / 2.f);
		configureText.setPosition(configureButton.getPosition() + configureButton.getSize() / 2.f);
	}

	void drawDerived(sf::RenderWindow& window) override {
		window.draw(configureButton);
		window.draw(configureText);
		nodeConfig.draw(window);
		if (nodeConfig.shouldDraw) {
			rec.draw(window);

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
		quantityText.setString(std::to_string(quantity));
		auto bounds = quantityText.getLocalBounds();
		quantityText.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });

		return true;
	}

	void toggleConfig() {
		nodeConfig.setPosition(getPosition() + size / 2.f - nodeConfig.getSize() / 2.f);
		nodeConfig.shouldDraw = !nodeConfig.shouldDraw;
		rec.setPosition(nodeConfig.background.getPosition() + sf::Vector2f(10.f, 30.f));
	}
};