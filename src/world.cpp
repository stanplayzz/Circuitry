#include "game.hpp"
#include "world.hpp"
#include <UltimateShapes/shapes.hpp>

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

void World::onEvent(sf::Event& event, sf::RenderWindow& window, Game& game, sf::View& worldView) {
	bool shouldUpdateView = false;
	if (auto mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse->button == sf::Mouse::Button::Right) {
			panning = true;
			prevMouse = sf::Mouse::getPosition(window);
		}
	}

	if (auto mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
		if (mouse->button == sf::Mouse::Button::Right) {
			panning = false;
		}
	}
	if (auto scroll = event.getIf<sf::Event::MouseWheelScrolled>()) {
		float zoomStep = (scroll->delta > 0) ? 0.9f : 1.1f;
		float newZoom = currentZoom * zoomStep;

		sf::Vector2f newSize = window.getDefaultView().getSize() * newZoom;
		sf::Vector2f worldSize(world_grid_size.x * tile_size, world_grid_size.y * tile_size);
		if (newSize.x > worldSize.x) newZoom = worldSize.x / window.getDefaultView().getSize().x;
		if (newSize.y > worldSize.y) newZoom = worldSize.y / window.getDefaultView().getSize().y;

		if (newZoom < 0.5f) newZoom = 0.5f;
		currentZoom = newZoom;
		
		worldView.setSize(window.getDefaultView().getSize() * currentZoom);
		shouldUpdateView = true;
	}
	if (panning) {
		auto mouse = sf::Mouse::getPosition(window);
		worldView.move(window.mapPixelToCoords(prevMouse) - window.mapPixelToCoords(mouse));
		prevMouse = mouse;
		shouldUpdateView = true;
	}

	if (shouldUpdateView) {
		auto half = worldView.getSize() / 2.f;
		auto center = worldView.getCenter();

		center.x = std::max(half.x, std::min(center.x, world_grid_size.x * tile_size - half.x));
		center.y = std::max(half.y, std::min(center.y, world_grid_size.y * tile_size - half.y));

		worldView.setCenter(center);
		window.setView(worldView);
	}

	nodeManager.onEvent(event, window, *this);
	playerHandler->onEvent(event, window, *this);
	toolbar->onEvent(event, window, game);
}

void World::draw(sf::RenderWindow& window, Game& game) {
	window.draw(grid);

	nodeManager.draw(window, *this);

	toolbar->draw(window, game, *this);
	// draw nodes that should be on top of everything
	if (playerHandler->currentNode)
		playerHandler->currentNode->draw(window);
}