#include "recipe.hpp"

#include <exception>
#include <print>

void fitTextToWidth(sf::Text& text, float maxWidth) {
	float width = text.getLocalBounds().size.x;
	if (width == 0.f) return;

	float scale = maxWidth / width;
	unsigned int newSize = std::max(1.f, text.getCharacterSize() * scale);

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
	inText.setCharacterSize(16);
	outText.setCharacterSize(16);
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
	outText.setPosition(position - outText.getLocalBounds().size / 2.f + sf::Vector2f(background.getSize().x / 2.f, background.getSize().y - outText.getLocalBounds().size.y / 2.f));
	inText.setOrigin(inText.getLocalBounds().size / 2.f);
	inText.setPosition(position + sf::Vector2f(background.getSize().x / 2.f, 
		background.getSize().y - inText.getLocalBounds().size.y - outText.getLocalBounds().size.y / 2.f));
}

void Recipe::draw(sf::RenderWindow& window) {
	window.draw(outline);
	window.draw(background);
	window.draw(inText);

	auto uiView = window.getDefaultView();
	auto worldView = window.getView();
	auto zoom = window.getSize().x / worldView.getSize().x;

	auto textWorldPos = outText.getPosition();
	auto textCharSize = outText.getCharacterSize();
	//auto textOrigin = outText.getOrigin();

	sf::Vector2i pixelPos = window.mapCoordsToPixel(textWorldPos, worldView);
	sf::Vector2f uiPos = window.mapPixelToCoords(pixelPos, window.getDefaultView());
	outText.setPosition(uiPos);
	//outText.setOrigin(textOrigin * zoom);
	outText.setCharacterSize(textCharSize * zoom);
	std::println("{}", std::round(textCharSize * zoom));
 
	window.setView(window.getDefaultView());
	window.draw(outText);
	window.setView(worldView);
	outText.setCharacterSize(textCharSize);
	//outText.setOrigin(textOrigin);
	outText.setPosition(textWorldPos);

}