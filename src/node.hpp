#pragma once
#include <SFML/Graphics.hpp>

class Node;

struct Port {
	sf::Vector2f offset{};
	bool isInput = false;
	sf::CircleShape shape{};
	Node* parentNode = nullptr;
	bool contains(sf::Vector2f position) const;
};

class Node {
public:
	bool inWorld = false;
	sf::Vector2f size{128.f, 192.f};
	std::vector<Port> ports;

	Node(sf::Vector2f position, sf::Texture& image, const int iPorts = 0, const int oPorts = 0);
	void setPosition(sf::Vector2f position);
	sf::Vector2f getPosition() const;
	sf::FloatRect getGlobalBounds() const;
	bool contains(sf::Vector2f position) const;
	void draw(sf::RenderWindow& window);

private:
	sf::RectangleShape background{};
	sf::Sprite image;
};