#pragma once
#include "node.hpp"

#include <SFML/Graphics.hpp>

class Game;
class World;

struct Card {
	sf::RectangleShape shape;
	bool inWorld = false;
};

class Inventory {
public:
	Inventory(Game& game);
	void update(World& world);
	void draw(sf::RenderWindow& window, Game& game);

	sf::RectangleShape inventoryButton;
	sf::RectangleShape inventory;
private:
	sf::RectangleShape bottomBar;

	sf::Vector2i cardSize = sf::Vector2i(3, 6);
	sf::Color cardColor = sf::Color(100, 100, 100);

};
