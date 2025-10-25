#include "toolbar.hpp"
#include "inventory.hpp"
#include "menu.hpp"
#include "./game.hpp"

#include <print>
#include <exception>

bool Button::isPressed(sf::Event& event) {
	if (auto mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
		if (mouse->button == sf::Mouse::Button::Left) {
			if (background.getGlobalBounds().contains(sf::Vector2f(mouse->position))) {
				return true;
			}
		}
	}

	return false;
}

void Button::onHover(sf::RenderWindow& window) {
	if (background.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
		background.setFillColor(sf::Color(255, 255, 255, 25));
	}
	else {
		background.setFillColor(sf::Color::Transparent);
	}
}

void Button::setPosition(sf::Vector2f position) {
	background.setPosition(position);
	auto bounds = text.getLocalBounds();
	text.setOrigin(bounds.position + bounds.size / 2.f);
	text.setPosition(position + background.getSize() / 2.f);
}

void Button::draw(sf::RenderWindow& window) {
	window.draw(background);
	window.draw(text);
}

Toolbar::Toolbar(Game& game) {
	if (!font.openFromFile(ASSETS_DIR + std::string("/fonts/Roboto.ttf"))) {
		throw std::runtime_error("Failed to open font");
	}

	toolbar.setSize({ (float)game.window.getSize().x, toolbarHeight });
	toolbar.setPosition({ 0.f, game.window.getSize().y - toolbarHeight});
	toolbar.setFillColor(sf::Color(50, 50, 120));


	menu = std::make_unique<Menu>(game, *this);
	inventory = std::make_unique<Inventory>(game, *this);
}

Toolbar::~Toolbar() = default;

void Toolbar::update(sf::Time deltaTime, World& world) {
	menu->update(deltaTime, world);
	inventory->update(deltaTime, world);
}

void Toolbar::onEvent(sf::Event& event, sf::RenderWindow& window, Game& game) {
	
	for (auto& button: buttons) {
		button->onHover(window);
	}
	inventory->onEvent(event, window, *this);
	menu->onEvent(event, window, *this, game);
}

void Toolbar::draw(sf::RenderWindow& window, Game& game, World& world) {
	window.setView(game.uiView);

	inventory->draw(window, world.nodeManager);
	menu->draw(window);
	window.draw(toolbar);
	for (auto& button : buttons) {
		button->draw(window);
	}

	window.setView(game.view);
}

void Toolbar::closeAll(sf::RenderWindow& window) {
	sf::Vector2f closedPos({0.f, (float)window.getSize().y});
	menu->targetPos = closedPos;
	menu->activated = false;
	inventory->targetPos = closedPos;
	inventory->activated = false;
}