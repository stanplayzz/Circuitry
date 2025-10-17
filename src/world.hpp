#pragma once
#include "playerHandler.hpp"
#include "nodeManager.hpp"
#include "node.hpp"

#include <SFML/Graphics.hpp>

class Game;

class World {
public:
	World(Game& game);
	void update(sf::RenderWindow& window);
	void onEvent(sf::Event& event, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window, Game& game);

	float tile_size = 32.f;
	sf::Vector2i world_grid_size = sf::Vector2i(100, 100);
	std::unique_ptr<PlayerHandler> playerHandler;
	std::unique_ptr<Inventory> inventory;
	NodeManager nodeManager;
private:
	sf::VertexArray grid;
};