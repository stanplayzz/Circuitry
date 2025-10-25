#include "gameState.hpp"
#include "mainMenuState.hpp"
#include "../game.hpp"

GameState::GameState(Game& game) : game(game), world(game) {

}

void GameState::onEvent(sf::Event& event) {
	world.onEvent(event, game.window, game, game.view);
	if (auto key = event.getIf<sf::Event::KeyReleased>()) {
		if (key->scancode == sf::Keyboard::Scancode::Escape) {
			game.stateManager.switchState(std::make_unique<MainMenu::MainMenuState>(game));
		}
	}
}

void GameState::update(sf::Time deltaTime) {
	world.update(game.window, deltaTime);
}

void GameState::draw(sf::RenderWindow& window) {
	window.clear(sf::Color(49, 51, 73));

	world.draw(window, game);
}