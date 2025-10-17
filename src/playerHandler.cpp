#include "playerHandler.hpp"
#include "game.hpp"

#include <print>

PlayerHandler::PlayerHandler(Game& game) : game(game) {

}

void PlayerHandler::update(sf::RenderWindow& window) {
	if (dragging && currentNode) {
		auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		currentNode->setPosition(mousePos - dragOffset);
	}
}

void PlayerHandler::onEvent(sf::Event& event, sf::RenderWindow& window, World& world) {
	auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	auto uiMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), game.uiView);

	if (auto mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse->button == sf::Mouse::Button::Left) {
			for (auto& node : world.nodeManager.nodes) {
				if (node.inWorld) {
					bool overPort = false;
					for (auto& port : node.ports) {
						if (port.contains(mousePos)) {
							overPort = true;
							currentPort = &port;
							currentNode = &node;
							break;
						}
					}
					if (overPort) break;
					
					if (node.contains(mousePos)) {
						currentNode = &node;
						dragging = true;
						dragOffset = mousePos - currentNode->getPosition();
						break;
					}
				}
				else {
					if (node.contains(uiMousePos)) {
						currentNode = &node;
						dragging = true;
						dragOffset = uiMousePos - currentNode->getPosition();
						currentNode->inWorld = true;
						break;
					}
				}
			}
		}
	}

	if (auto mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
		if (mouse->button == sf::Mouse::Button::Left) {
			if (dragging && currentNode) {
				dragging = false;

				if (!world.inventory->inventory.getGlobalBounds().contains(uiMousePos)) {
					sf::Vector2f gridPos(std::round((mousePos.x - dragOffset.x) / game.world->tile_size),
										 std::round((mousePos.y - dragOffset.y) / game.world->tile_size));
					currentNode->setPosition(gridPos * game.world->tile_size);
					currentNode->inWorld = true;
					world.inventory->update(world);
				}
				else {
					currentNode->inWorld = false;
					world.inventory->update(world);
				}

				currentNode = nullptr;
			}
			else if (currentPort) {
				for (auto& node : world.nodeManager.nodes) {
					if (node.inWorld && &node != currentNode) {
						for (auto& port : node.ports) {
							if (port.contains(mousePos)) {
								world.nodeManager.createConnection(currentNode, &node, currentPort, &port);
								std::println("yes");
							}
						}
					}
				}
			}
		}
		
	}
}