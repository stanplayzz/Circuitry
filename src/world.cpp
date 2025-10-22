#include "game.hpp"
#include "world.hpp"

auto gridColor = sf::Color(255, 255, 255, 25);

World::World(Game& game) {
	playerHandler = std::make_unique<PlayerHandler>(game);
	toolbar = std::make_unique<Toolbar>(game);

	grid.setPrimitiveType(sf::PrimitiveType::Lines);

	for (float x = 0; x <= world_grid_size.x; x++) {
		grid.append(sf::Vertex({ x * tile_size, 0.f }, gridColor));
		grid.append(sf::Vertex({ x * tile_size, (float)world_grid_size.x * tile_size }, gridColor));
	}
	for (float y = 0; y <= world_grid_size.y; y++) {
		grid.append(sf::Vertex({ 0.f, y * tile_size }, gridColor));
		grid.append(sf::Vertex({ (float)world_grid_size.y * tile_size, y * tile_size }, gridColor));
	}
}

void World::update(sf::RenderWindow& window, sf::Time deltaTime) {
	playerHandler->update(window, nodeManager);
	nodeManager.update(deltaTime,  window);
	toolbar->update(deltaTime, *this);
}

void World::onEvent(sf::Event& event, sf::RenderWindow& window) {
	nodeManager.onEvent(event, window, *this);
	playerHandler->onEvent(event, window, *this);
	toolbar->onEvent(event, window);
}

void World::draw(sf::RenderWindow& window, Game& game) {
	window.draw(grid);

	nodeManager.draw(window, game);

	toolbar->draw(window, game);
	// draw nodes that should be on top of everything
	if (game.world->playerHandler->currentNode)
		game.world->playerHandler->currentNode->draw(window);
}