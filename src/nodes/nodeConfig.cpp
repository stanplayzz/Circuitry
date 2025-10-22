#include "nodeConfig.hpp"
#include "../world.hpp"

#include <print>

NodeConfig::NodeConfig(sf::Font& font) : font(font) {
	titleBackground.setCorners(us::Corner::TopLeft | us::Corner::TopRight);
}

void NodeConfig::setPosition(sf::Vector2f position) {
	background.setPosition(position);
	background.setFillColor(sf::Color(100, 100, 100));
	titleBackground.setPosition(position);
	titleBackground.setFillColor(sf::Color(120, 120, 120));
	auto bounds = title.getLocalBounds();
	title.setOrigin(bounds.size / 2.f);
	title.setPosition(position + sf::Vector2f(background.getSize().x / 2.f, bounds.size.y / 2.f));
}

[[nodiscard]] sf::Vector2f NodeConfig::getSize() const {
	return background.getSize();
}

void NodeConfig::draw(sf::RenderWindow& window){
	if (shouldDraw) {
		window.draw(background);
		window.draw(titleBackground);
		window.draw(title);
		for (auto& drawable : drawables) {
			window.draw(drawable);
		}
	}
}