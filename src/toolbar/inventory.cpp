#include "inventory.hpp"
#include "./game.hpp"
#include "./world.hpp"

#include <print>

Inventory::Inventory(Game& game, Toolbar& toolbar) 
	: button(toolbar.font, sf::Vector2f(toolbar.buttonWidth, toolbar.toolbarHeight), "Inventory") {
	inventory.setPosition({ 0.f, (float)game.window.getSize().y });
	targetPos = { 0.f, (float)game.window.getSize().y };
	inventory.setSize(sf::Vector2f((float)game.window.getSize().x, 224.f + toolbar.toolbarHeight));
	inventory.setFillColor(sf::Color(60, 60, 150));

	auto index = toolbar.buttons.size();
	button.setPosition(sf::Vector2f(10.f + toolbar.buttonWidth * index, toolbar.toolbar.getPosition().y));

	toolbar.buttons.push_back(&button);
}

void Inventory::update(sf::Time deltaTime, World& world) {
	if (inventory.getPosition() != targetPos) {
		auto deltaPos = targetPos - inventory.getPosition();

		if (std::sqrt(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y) < 1.0f) {
			inventory.setPosition(targetPos);
		}
		else {
			inventory.setPosition(inventory.getPosition() + deltaPos * 5.f * deltaTime.asSeconds());
		}
	}

	int index = 0;
	for (auto& node : world.nodeManager.nodes) {
		if (!node->inWorld) {
			node->setPosition(inventory.getPosition() + sf::Vector2f(index * 144.f + 16.f, 16.f));
			index++;
		}
	}
}

void Inventory::onEvent(sf::Event& event, sf::RenderWindow& window, Toolbar& toolbar) {
	if (button.isPressed(event)) {
		if (activated) {
			targetPos = { 0.f, (float)window.getSize().y };
		}
		else {
			toolbar.closeAll(window);
			targetPos = { 0.f, (float)window.getSize().y - inventory.getSize().y };
		}
		activated = !activated;
	}
}

void Inventory::draw(sf::RenderWindow& window, NodeManager& nodeManager) {
	window.draw(inventory);
	for (auto& node : nodeManager.nodes) {
		if (!node->inWorld) {
			node->draw(window);
		}
	}
}