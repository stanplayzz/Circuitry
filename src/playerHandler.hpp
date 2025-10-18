#pragma once
#include "inventory.hpp"
#include "node.hpp"

#include <SFML/Graphics.hpp>

class Game;
class World;
class NodeManager;

class PlayerHandler {
public:
	PlayerHandler(Game& game);
	void update(sf::RenderWindow& window, NodeManager& nodeManager);
	void onEvent(sf::Event& event, sf::RenderWindow& window, World& world);

	Node* currentNode = nullptr;
	bool dragging = false;
	Port* currentPort = nullptr;
private:
	Game& game;

	sf::Vector2f dragOffset{};
};