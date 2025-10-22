#pragma once
#include "toolbar.hpp"

#include <SFML/Graphics.hpp>

class Game;

class Menu {
public:
	Menu(Game& game, Toolbar& toolbar);
	void update(sf::Time deltaTime, World& world);
	void onEvent(sf::Event& event, sf::RenderWindow& window, Toolbar& toolbar);
	void draw(sf::RenderWindow& window);

	sf::Vector2f targetPos{};
	bool activated = false;
	bool canClick = false;
private:
	sf::RectangleShape menu;
	Button button;
	Button quitButton;
	std::vector<Button*> menuButtons;
};