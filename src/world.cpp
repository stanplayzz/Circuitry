#include "game.hpp"
#include "world.hpp"

auto gridColor = sf::Color(255, 255, 255, 25);

World::World(Game& game) {
	playerHandler = std::make_unique<PlayerHandler>(game);
	inventory = std::make_unique<Inventory>(game);

	grid.setPrimitiveType(sf::PrimitiveType::Lines);

	for (float x = 0; x <= world_grid_size.x; x++) {
		grid.append(sf::Vertex({ x * tile_size, 0.f }, gridColor));
		grid.append(sf::Vertex({ x * tile_size, (float)world_grid_size.x * tile_size }, gridColor));
	}
	for (float y = 0; y <= world_grid_size.y; y++) {
		grid.append(sf::Vertex({ 0.f, y * tile_size }, gridColor));
		grid.append(sf::Vertex({ (float)world_grid_size.y * tile_size, y * tile_size }, gridColor));
	}



	inventory->update(*this);
}

void World::update(sf::RenderWindow& window, sf::Time deltaTime) {
	playerHandler->update(window, nodeManager);
	nodeManager.update(deltaTime);
}

void World::onEvent(sf::Event& event, sf::RenderWindow& window) {
	playerHandler->onEvent(event, window, *this);
	nodeManager.onEvent(event, *this);
}

void World::draw(sf::RenderWindow& window, Game& game) {
	window.draw(grid);

	for (auto& connection : nodeManager.connections) {
		connection.draw(window);
	}
	for (auto& node : nodeManager.nodes) {
		if (node->inWorld && playerHandler->currentNode != node.get()) {
			node->draw(window);
		}
	}

	inventory->draw(window, game);

	window.setView(game.uiView);
	for (auto& node : nodeManager.nodes) {
		if (!node->inWorld) {
			node->draw(window);
		}
	}
	window.setView(game.view);
	if (playerHandler->currentNode)
		playerHandler->currentNode->draw(window);

}