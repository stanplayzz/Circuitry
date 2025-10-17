#include "game.hpp"

Game::Game() {
	window.create(sf::VideoMode({ 1280, 720 }), "I'll change this later", sf::Style::Close);
	uiView = window.getDefaultView();
	world = std::make_unique<World>(*this);
	view = sf::View(sf::FloatRect(sf::Vector2f(world->world_grid_size * (int)world->tile_size) / 2.f, sf::Vector2f(window.getSize())));
	window.setView(view);
	originalViewSize = view.getSize();

	run();
}

void Game::run() {
	while (window.isOpen()) {
		handleInput();
		
		world->update(window);

		window.clear(sf::Color(75, 75, 200));

		world->draw(window, *this);

		window.display();
	}
}

void Game::handleInput() {
	bool shouldUpdateView = false;
	while (auto event = window.pollEvent()) {
		world->onEvent(*event, window);

		if (event->is<sf::Event::Closed>())
			window.close();
		if (auto mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
			if (mouse->button == sf::Mouse::Button::Right) {
				panning = true;
				prevMouse = sf::Mouse::getPosition(window);
			}
		}

		if (auto mouse = event->getIf<sf::Event::MouseButtonReleased>()) {
			if (mouse->button == sf::Mouse::Button::Right) {
				panning = false;
			}
		}
		if (auto scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
			float zoomStep = (scroll->delta > 0) ? 0.9f : 1.1f;
			float newZoom = currentZoom * zoomStep;

			sf::Vector2f newSize = originalViewSize * newZoom;
			sf::Vector2f worldSize(world->world_grid_size.x * world->tile_size, world->world_grid_size.y * world->tile_size);
			if (newSize.x > worldSize.x) newZoom = worldSize.x / originalViewSize.x;
			if (newSize.y > worldSize.y) newZoom = worldSize.y / originalViewSize.y;

			if (newZoom < 0.5f) newZoom = 0.5f;
			currentZoom = newZoom;
			view.setSize(originalViewSize * currentZoom);
			shouldUpdateView = true;
		}
	}
	if (panning) {
		auto mouse = sf::Mouse::getPosition(window);
		view.move(window.mapPixelToCoords(prevMouse) - window.mapPixelToCoords(mouse));
		prevMouse = mouse;
		shouldUpdateView = true;
	}

	if (shouldUpdateView) {
		auto half = view.getSize() / 2.f;
		auto center = view.getCenter();

		center.x = std::max(half.x, std::min(center.x, world->world_grid_size.x * world->tile_size - half.x));
		center.y = std::max(half.y, std::min(center.y, world->world_grid_size.y * world->tile_size - half.y));

		view.setCenter(center);
		window.setView(view);
	}
}