#pragma once 
#include "resources.hpp"

#include <SFML/Graphics.hpp>
#include <UltimateShapes/shapes.hpp>

class Recipe {
public:
	Recipe(Resource& iRes, Resource& oRes, int inRate, int outRate);
	void setSize(sf::Vector2f size);
	void setPosition(sf::Vector2f position);
	void draw(sf::RenderWindow& window);
private:
	us::RoundedRectangleShape background;
	us::RoundedRectangleShape outline;
	sf::Font font;
	sf::Text inText{ font };
	sf::Text outText{ font };

	float outlineThickness = 3.f;
};