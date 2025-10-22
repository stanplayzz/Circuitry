#pragma once
#include "toolbar.hpp"
#include "./nodes/node.hpp"

#include <SFML/Graphics.hpp>

class Game;
class World;


class Inventory {
public:
	Inventory(Game& game, Toolbar& toolbar);
	void update(sf::Time deltaTime, World& world);
	void onEvent(sf::Event& event, sf::RenderWindow& window, Toolbar& toolbar);
	void draw(sf::RenderWindow& window, NodeManager& nodeManager);
	void close();

	sf::Vector2f targetPos{};
	sf::RectangleShape inventory;
	bool activated = false;
private:
	Button button;
};
