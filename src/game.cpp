#include "game.hpp"

#include <print>

Game::Game() : fps(font) {
	window.create(sf::VideoMode({ 1280, 720 }), "I'll change this later", sf::Style::Close);
	uiView = window.getDefaultView();
	world = std::make_unique<World>(*this);
	view = sf::View(sf::FloatRect(sf::Vector2f(world->world_grid_size * (int)world->tile_size) / 2.f, sf::Vector2f(window.getSize())));
	window.setView(view);
	originalViewSize = view.getSize();
	window.setVerticalSyncEnabled(true);

	font.openFromFile(ASSETS_DIR + std::string("/fonts/Roboto.ttf"));
	fps.setCharacterSize(30);
	fps.setFillColor(sf::Color::Black);

	run();
}

void Game::run() {
	sf::Clock fpsClock;
	sf::Clock deltaClock;
	int frameCount = 0;
	int cfps = 0.f;
	while (window.isOpen()) {
		sf::Time deltaTime = deltaClock.restart();
		frameCount++;
		if (fpsClock.getElapsedTime().asSeconds() >= 1.f) {
			cfps = frameCount / fpsClock.getElapsedTime().asSeconds();
			fpsClock.restart();
			frameCount = 0;
			fps.setString(std::to_string(cfps));
		}

		handleInput();
		
		world->update(window, deltaTime);

		window.clear(sf::Color(49, 51, 73));

		world->draw(window, *this);
		window.setView(uiView);
		window.draw(fps);
		window.setView(view);


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