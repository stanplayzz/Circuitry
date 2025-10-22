#include "recipe.hpp"

#include <exception>

void fitTextToWidth(sf::Text& text, float maxWidth) {
	float width = text.getLocalBounds().size.x;
	if (width == 0.f) return;

	float scale = maxWidth / width;
	unsigned int newSize = static_cast<unsigned int>(std::max(1.f, text.getCharacterSize() * scale));

	text.setCharacterSize(newSize);
}

Recipe::Recipe(Resource& iRes, Resource& oRes, int inRate, int outRate) {
	if (!font.openFromFile(ASSETS_DIR + std::string("/fonts/Roboto.ttf"))) {
		throw std::runtime_error("Failed to open font");
	}

	outline.setFillColor(sf::Color(70, 70, 70));
	background.setFillColor(sf::Color(100, 100, 100));
	outline.setCornerRadius(8.f);
	background.setCornerRadius(5.f);
	inText.setString(std::format("{} : {}/min", iRes.as_string(), inRate * 60.f));
	outText.setString(std::format("{} : {}/min", oRes.as_string(), outRate * 60.f));
	inText.setCharacterSize(60);
	outText.setCharacterSize(60);
}

void Recipe::setSize(sf::Vector2f size) {
	outline.setSize(size);
	background.setSize(size - sf::Vector2f(outlineThickness, outlineThickness) * 2.f);
	fitTextToWidth(inText, size.x - outlineThickness);
	fitTextToWidth(outText, size.x - outlineThickness);
}

void Recipe::setPosition(sf::Vector2f position) {
	outline.setPosition(position);
	background.setPosition(position + sf::Vector2f(outlineThickness, outlineThickness));
	outText.setOrigin(outText.getLocalBounds().size / 2.f);
	outText.setPosition(position + sf::Vector2f(background.getSize().x / 2.f, background.getSize().y - outText.getLocalBounds().size.y / 2.f));
	inText.setOrigin(inText.getLocalBounds().size / 2.f);
	inText.setPosition(position + sf::Vector2f(background.getSize().x / 2.f, 
		background.getSize().y - inText.getLocalBounds().size.y - outText.getLocalBounds().size.y / 2.f));
}

void Recipe::draw(sf::RenderWindow& window) {
	window.draw(outline);
	window.draw(background);
	window.draw(inText);
	window.draw(outText);
}