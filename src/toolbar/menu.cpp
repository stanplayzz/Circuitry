#include "menu.hpp"
#include "../game.hpp"

Menu::Menu(Game& game, Toolbar& toolbar)
	: button(toolbar.font, sf::Vector2f(toolbar.buttonWidth, toolbar.toolbarHeight), "Menu"),
	  quitButton(toolbar.font, sf::Vector2f(toolbar.buttonWidth * 2.f, toolbar.toolbarHeight), "Quit To Desktop") {
	menuButtons.push_back(&quitButton);

	menu.setSize({ toolbar.buttonWidth * 2.f, toolbar.toolbarHeight * menuButtons.size() + toolbar.toolbarHeight });
	menu.setPosition({ 0.f, (float)game.window.getSize().y});
	targetPos = menu.getPosition();
	menu.setFillColor(sf::Color(60, 60, 150));

	for (size_t i = 0; i < menuButtons.size(); i++) {
		menuButtons[i]->setPosition({ 0.f, menu.getPosition().y + i * toolbar.toolbarHeight});
	}

	auto index = toolbar.buttons.size();
	button.setPosition(sf::Vector2f(10.f + toolbar.buttonWidth * index, toolbar.toolbar.getPosition().y));

	toolbar.buttons.push_back(&button);
}

void Menu::update(sf::Time deltaTime, World& world) {
	if (menu.getPosition() != targetPos) {
		auto deltaPos = targetPos - menu.getPosition();

		if (std::sqrt(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y) < 1.0f) {
			menu.setPosition(targetPos);
			canClick = !canClick;
		}
		else {
			menu.setPosition(menu.getPosition() + deltaPos * 5.f * deltaTime.asSeconds());
		}
	}

	for (size_t i = 0; i < menuButtons.size(); i++) {
		menuButtons[i]->setPosition({ 0.f, menu.getPosition().y + i * world.toolbar->toolbarHeight });
	}
}

void Menu::onEvent(sf::Event& event, sf::RenderWindow& window, Toolbar& toolbar) {
	for (auto& button : menuButtons) {
		button->onHover(window);
	}

	if (button.isPressed(event)) {
		if (activated) {
			targetPos = { 0.f, (float)window.getSize().y };
		}
		else {
			toolbar.closeAll(window);
			targetPos = { 0.f, (float)window.getSize().y - menu.getSize().y };
		}
		activated = !activated;
	}

	if (quitButton.isPressed(event) && canClick) {
		window.close();
	}
}

void Menu::draw(sf::RenderWindow& window) {
	window.draw(menu);
	for (auto& button : menuButtons) {
		button->draw(window);
	}
}