#pragma once

#include <SFML/Graphics.hpp>
#include <UltimateShapes/shapes.hpp>

class NodeConfig {
private:
	sf::Font& font;
public:
	NodeConfig(sf::Font& font);
	void setPosition(sf::Vector2f position);
	sf::Vector2f getSize() const;

	void draw(sf::RenderWindow& window);

	bool shouldDraw = false;
	us::RoundedRectangleShape background{ {192.f, 96.f}, 10.f };
	us::SelectiveRoundedRectangleShape titleBackground{ {192.f, 20.f}, 10.f };
	sf::Text title{font};
	std::vector<sf::Drawable> drawables;
};