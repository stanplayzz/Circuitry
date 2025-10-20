#include "playerHandler.hpp"
#include "game.hpp"

#include <print>

PlayerHandler::PlayerHandler(Game& game) : game(game) {

}

void PlayerHandler::update(sf::RenderWindow& window, NodeManager& nodeManager) {
	if (dragging && currentNode) {
		auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		currentNode->setPosition(mousePos - dragOffset);
		nodeManager.updateNode(currentNode);
	}
}

void PlayerHandler::onEvent(sf::Event& event, sf::RenderWindow& window, World& world) {
	auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	auto uiMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), game.uiView);

	if (auto mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse->button == sf::Mouse::Button::Left) {
			for (auto& node : world.nodeManager.nodes) {
				if (node->inWorld) {
					bool overPort = false;
					for (auto& port : node->ports) {
						if (port.contains(mousePos)) {
							overPort = true;
							currentPort = &port;
							currentNode = node.get();
							break;
						}
					}
					if (overPort) break;
					
					if (node->contains(mousePos)) {
						currentNode = node.get();
						dragging = true;
						dragOffset = mousePos - currentNode->getPosition();
						break;
					}
				}
				else {
					if (node->contains(uiMousePos)) {
						currentNode = node.get();
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
					const float padding = 32.f;
					const float tile = game.world->tile_size;

					sf::Vector2f gridPos(
						std::round((mousePos.x - dragOffset.x) / tile) * tile,
						std::round((mousePos.y - dragOffset.y) / tile) * tile
					);

					sf::FloatRect newBounds(
						gridPos - sf::Vector2f(padding, padding),
						currentNode->size + sf::Vector2f(padding * 2.f, padding * 2.f)
					);

					auto rectsOverlap = [](const sf::FloatRect& a, const sf::FloatRect& b) -> bool {
						return a.position.x < b.position.x + b.size.x &&
							a.position.x + a.size.x > b.position.x &&
							a.position.y < b.position.y + b.size.y &&
							a.position.y + a.size.y > b.position.y;
						};

					bool canPlace = true;
					for (auto& node : world.nodeManager.nodes) {
						if (node.get() == currentNode) continue;
						if (!node->inWorld) continue;

						if (rectsOverlap(newBounds, node->getGlobalBounds())) {
							canPlace = false;
							break;
						}
					}

					if (canPlace) {
						currentNode->setPosition(gridPos);
						currentNode->inWorld = true;
						world.inventory->update(world);
					}
					else {
						// cannot place here
						currentNode->inWorld = false;
						world.inventory->update(world);

						auto connectionsToRemove = world.nodeManager.getNodeConnections(currentNode);
						for (int i = connectionsToRemove.size() - 1; i >= 0; --i) {
							world.nodeManager.removeConnection(connectionsToRemove[i]);
						}
					}
				}
				else {
					// in inventory
					currentNode->inWorld = false;
					world.inventory->update(world);
					auto connectionsToRemove = world.nodeManager.getNodeConnections(currentNode);
					for (int i = connectionsToRemove.size() - 1; i >= 0; --i) {
						world.nodeManager.removeConnection(connectionsToRemove[i]);
					}
				}

				world.nodeManager.updateNode(currentNode);
			}
			else if (currentPort) {
				for (auto& node : world.nodeManager.nodes) {
					if (node->inWorld && node.get() != currentNode) {
						for (auto& port : node->ports) {
							if (port.contains(mousePos)) {
								if (!currentPort->isInput == port.isInput) {
									world.nodeManager.createConnection(currentNode, node.get(), currentPort, &port);
								}
							}
						}
					}
				}
			}
			currentNode = nullptr;
		}
	}
}