#include "mainMenuState.hpp"
#include "gameState.hpp"
#include "../game.hpp"

#include <exception>
#include <print>


namespace MainMenu {
	MainMenuState::MainMenuState(Game& game) : game(game) {
		backgroundShape.setSize(sf::Vector2f(game.window.getSize()));
		if (!backgroundTexture.loadFromFile(ASSETS_DIR + std::string("/textures/MainMenuBG.png"))) {
			throw std::runtime_error("Failed to load texture!");
		}
		backgroundShape.setTexture(&backgroundTexture);

		if (!TitanOne_font.openFromFile(ASSETS_DIR + std::string("/fonts/TitanOne.ttf"))) {
			throw std::runtime_error("Failed to open font!");
		}
		if (!playTexture.loadFromFile(ASSETS_DIR + std::string("/textures/PlayButton.png"))) {
			throw std::runtime_error("Failed to load texture!");
		}
		playButton.shape.setTexture(&playTexture, true);
		if (!quitTexture.loadFromFile(ASSETS_DIR + std::string("/textures/QuitButton.png"))) {
			throw std::runtime_error("Failed to load texture!");
		}
		quitButton.shape.setTexture(&quitTexture, true);

		title.setCharacterSize(100);
		title.setString("CIRCUITRY");
		title.setOrigin(title.getLocalBounds().position + title.getLocalBounds().size / 2.f);
		title.setPosition({ game.window.getSize().x / 2.f, 150.f });
		playButton.setSize({ 200.f, 50.f });
		playButton.setPosition({100.f, 300.f});
		playButton.hitBox = { playButton.position, playButton.getSize() };
		quitButton.setSize({ 200.f, 50.f });
		quitButton.setPosition({ 100.f, 375.f });
		quitButton.hitBox = { quitButton.position, quitButton.getSize() };
	}

	void MainMenuState::onEvent(sf::Event& event) {
		if (auto mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
			if (playButton.hitBox.contains(sf::Vector2f(mouse->position))) {
				game.stateManager.switchState(std::make_unique<GameState>(game));
			}
			if (quitButton.hitBox.contains(sf::Vector2f(mouse->position))) {
				game.window.close();
			}
		}
	}

	void MainMenuState::update(sf::Time deltaTime) {
		if (playButton.hitBox.contains(sf::Vector2f(sf::Mouse::getPosition(game.window)))) {
			playButton.targetPos = playButton.hitBox.position + sf::Vector2f(20.f, 0.f);
		}
		else {
			playButton.targetPos = playButton.hitBox.position;
		}
		if (quitButton.hitBox.contains(sf::Vector2f(sf::Mouse::getPosition(game.window)))) {
			quitButton.targetPos = quitButton.hitBox.position + sf::Vector2f(20.f, 0.f);
		}
		else {
			quitButton.targetPos = quitButton.hitBox.position;
		}
		playButton.shape.setPosition(playButton.shape.getPosition() + (playButton.targetPos - playButton.shape.getPosition()) * 10.f * deltaTime.asSeconds());
		quitButton.shape.setPosition(quitButton.shape.getPosition() + (quitButton.targetPos - quitButton.shape.getPosition()) * 10.f * deltaTime.asSeconds());
	}

	void MainMenuState::draw(sf::RenderWindow& window) {
		window.clear();
		window.setView(game.uiView);

		window.draw(backgroundShape);
		window.draw(title);
		playButton.draw(window);
		quitButton.draw(window);

		window.setView(game.view);
	}

	void Button::setSize(sf::Vector2f size) {
		shape.setSize(size);
		shape.setCornerRadius(10.f);
	}

	void Button::setPosition(sf::Vector2f pos) {
		shape.setPosition(pos);
		position = pos;
	}
	sf::Vector2f Button::getSize() const {
		return shape.getSize();
	}
	void Button::draw(sf::RenderWindow& window) {
		window.draw(shape);
	}
}
