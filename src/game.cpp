#include "game.hpp"
#include "states/gameState.hpp"
#include "states/mainMenuState.hpp"

#include <print>

sf::Vector2i world_grid_size(100, 100);
int tile_size = 32;

Game::Game() : fps(font) {
	window.create(sf::VideoMode({ 1280, 720 }), "I'll change this later", sf::Style::Close);
	uiView = window.getDefaultView();
	view = sf::View(sf::FloatRect(sf::Vector2f(world_grid_size * tile_size) / 2.f, sf::Vector2f(window.getSize())));
	window.setView(view);
	window.setVerticalSyncEnabled(true);

	font.openFromFile(ASSETS_DIR + std::string("/fonts/Roboto.ttf"));
	fps.setCharacterSize(30);
	fps.setFillColor(sf::Color::Black);

	stateManager.switchState(std::make_unique<MainMenu::MainMenuState>(*this));
}

void Game::run() {
	sf::Clock fpsClock;
	sf::Clock deltaClock;
	int frameCount = 0;
	int cfps = 0;
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
		
		stateManager.update(deltaTime);

		stateManager.draw(window);
		window.setView(uiView);
		window.draw(fps);
		window.setView(view);


		window.display();
	}
}

void Game::handleInput() {
	while (auto event = window.pollEvent()) {
		stateManager.onEvent(*event);

		if (event->is<sf::Event::Closed>())
			window.close();
	}
}