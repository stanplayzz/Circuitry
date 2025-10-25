#pragma once
#include "playerHandler.hpp"
#include "nodes/node.hpp"
#include "nodes/nodeManager.hpp"
#include "toolbar/toolbar.hpp"

#include <SFML/Graphics.hpp>

class Game;

class World {
public:
	World(Game& game);
	void update(sf::RenderWindow& window, sf::Time deltaTime);
	void onEvent(sf::Event& event, sf::RenderWindow& window, Game& game, sf::View& worldView);
	void draw(sf::RenderWindow& window, Game& game);

	float tile_size = 32.f;
	sf::Vector2i world_grid_size = sf::Vector2i(100, 100);
	std::unique_ptr<PlayerHandler> playerHandler;
	std::unique_ptr<Toolbar> toolbar;
	NodeManager nodeManager;
private:
	sf::VertexArray grid;
	bool panning = false;
	sf::Vector2i prevMouse{};
	float currentZoom = 1.f;
};