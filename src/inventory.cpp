#include "inventory.hpp"
#include "game.hpp"
#include "world.hpp"

#include <print>

Inventory::Inventory(Game& game) {
	bottomBar.setSize(sf::Vector2f((float)game.window.getSize().x, 20.f));
	bottomBar.setFillColor(sf::Color(47, 66, 150));
	bottomBar.setPosition(sf::Vector2f(0.f, game.window.getSize().y -20.f));

	inventoryButton.setSize(sf::Vector2f(40.f, 20.f));
	inventoryButton.setFillColor(sf::Color::Blue);
	inventoryButton.setPosition(sf::Vector2f(20.f, game.window.getSize().y - 20.f));

	inventory.setSize(sf::Vector2f((float)game.window.getSize().x, 224.f + bottomBar.getSize().y));
	inventory.setPosition(sf::Vector2f(0.f, (float)game.window.getSize().y - (float)inventory.getSize().y));
	inventory.setFillColor(sf::Color::White);
}

void Inventory::update(World& world) {
	int index = 0;
	for (auto& node : world.nodeManager.nodes) {
		if (!node.inWorld) {
			node.setPosition(world.inventory->inventory.getPosition() + sf::Vector2f(index * 144.f + 16.f, 16.f));
			index++;
		}
	}
}

void Inventory::draw(sf::RenderWindow& window, Game& game) {
	window.setView(game.uiView);

	window.draw(inventory);

	window.draw(bottomBar);
	window.draw(inventoryButton);
	
	window.setView(game.view);

}