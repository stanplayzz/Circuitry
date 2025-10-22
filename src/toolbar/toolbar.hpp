#pragma once

#include <SFML/Graphics.hpp>

class Game;
class World;
class Inventory;
class Menu;

struct Button {
	sf::RectangleShape background;
	sf::Text text;

	Button(sf::Font& font, const sf::Vector2f size, const std::string& string) :text(font), background(size) {
		background.setSize(size);
		background.setFillColor(sf::Color::Transparent);
		text.setString(string);
		text.setCharacterSize(20);
	}

	bool isPressed(sf::Event& event);
	void onHover(sf::RenderWindow& window);
	void setPosition(sf::Vector2f position);
	void draw(sf::RenderWindow& window);
};

class Toolbar {
public:
	float toolbarHeight = 28.f;
	float buttonWidth = 128.f;
	sf::Font font;
	sf::RectangleShape toolbar;

	std::vector<Button*> buttons;

	Toolbar(Game& game);
	~Toolbar();

	void closeAll(sf::RenderWindow& window);
	void update(sf::Time deltaTime, World& world);
	void onEvent(sf::Event& event, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window, Game& game);

	std::unique_ptr<Menu> menu;
	std::unique_ptr<Inventory> inventory;
};