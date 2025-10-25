#include "nodes/node.hpp"
#include "nodes/nodeManager.hpp"

#include <print>

Node::Node(NodeManager& nodeManager, const int iPorts, const int oPorts)
	: titleText(nodeManager.font), nodeManager(nodeManager), image(texture) {
	background.setSize(size);
	background.setCornerRadius(6.f);
	background.setCornerPointCount(2);
	
	titleBackground.setSize({size.x, 24.f});
	titleBackground.setCorners(us::Corner::TopLeft | us::Corner::TopRight);
	titleBackground.setCornerRadius(6.f);
	titleBackground.setCornerPointCount(2);

	image.setScale({ 0.125f, 0.125f });
	titleText.setCharacterSize(20.f);
	titleText.setFillColor(sf::Color::White);

	for (int i = 0; i < iPorts; i++) {
		float y = (i + 1) * size.y / (iPorts + 1);
		ports.push_back(Port{sf::Vector2f(0.f, y), true});
	}
	for (int i = 0; i < oPorts; i++) {
		float y = (i + 1) * size.y / (oPorts + 1);
		ports.push_back(Port{ sf::Vector2f(size.x, y), false});
	}

	for (auto& port : ports) {
		port.shape.setRadius(8.f);
		port.shape.setFillColor(port.isInput ? sf::Color::Green : sf::Color::Red);
		port.parentNode = this;
	}
}

void Node::setPosition(sf::Vector2f position) {
	background.setPosition(position);
	titleBackground.setPosition(position);
	auto offset = (size.x - 512.f * image.getScale().x) / 2.f;
	image.setPosition(position + size / 2.f - image.getGlobalBounds().size / 2.f);

	titleText.setOrigin(titleText.getLocalBounds().position + titleText.getLocalBounds().size / 2.f);
	titleText.setPosition(position + titleBackground.getGeometricCenter());

	for (auto& port : ports) {
		port.shape.setPosition(position + port.offset - sf::Vector2f(port.shape.getRadius(), port.shape.getRadius()));
	}
}

[[nodiscard]] sf::Vector2f Node::getPosition() const {
	return background.getPosition();
}

[[nodiscard]] bool Node::contains(sf::Vector2f position) const {
	return background.getGlobalBounds().contains(position);
}

void Node::draw(sf::RenderWindow& window) {
	window.draw(background);
	window.draw(titleBackground); 
	window.draw(image);
	window.draw(titleText);
	for (auto& port : ports)
		window.draw(port.shape);
}

[[nodiscard]] bool Port::contains(sf::Vector2f position) const {
	return shape.getGlobalBounds().contains(position);
}

[[nodiscard]] sf::FloatRect Node::getGlobalBounds() const {
	return background.getGlobalBounds();
}