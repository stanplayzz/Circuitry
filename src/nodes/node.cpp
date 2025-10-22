#include "nodes/node.hpp"
#include "nodes/nodeManager.hpp"

Node::Node(sf::Texture& texture, NodeManager& nodeManager, const int iPorts, const int oPorts)
	: image(texture), font(nodeManager.font), quantityText(font), nameText(font), nodeManager(nodeManager) {
	background.setSize(size);
	background.setFillColor(sf::Color(40, 40, 40));
	image.setScale({ 0.2f, 0.2f });
	quantityText.setCharacterSize(30.f);
	quantityText.setFillColor(sf::Color(200, 200, 200));
	nameText.setCharacterSize(20.f);
	nameText.setFillColor(sf::Color::White);

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
	auto offset = (size.x - 512.f * image.getScale().x) / 2.f;
	image.setPosition(position + sf::Vector2f(offset, offset + 20.f));

	auto bounds = quantityText.getLocalBounds();
	quantityText.setOrigin( bounds.size / 2.f);
	quantityText.setPosition({ position + size / 2.f + sf::Vector2f(0.f, 20.f) });
	bounds = nameText.getLocalBounds();
	nameText.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
	nameText.setPosition(position + sf::Vector2f(size.x / 2.f, offset + 10.f - (bounds.size.y / 2.f )));

	for (auto& port : ports) {
		port.shape.setPosition(position + port.offset - sf::Vector2f(port.shape.getRadius(), port.shape.getRadius()));
	}

	setDerivedPosition(position);
}

[[nodiscard]] sf::Vector2f Node::getPosition() const {
	return background.getPosition();
}

[[nodiscard]] bool Node::contains(sf::Vector2f position) const {
	return background.getGlobalBounds().contains(position);
}

void Node::draw(sf::RenderWindow& window) {
	window.draw(background);
	window.draw(image);
	window.draw(quantityText);
	window.draw(nameText);
	for (auto& port : ports)
		window.draw(port.shape);
	drawDerived(window);
}

[[nodiscard]] bool Port::contains(sf::Vector2f position) const {
	return shape.getGlobalBounds().contains(position);
}

[[nodiscard]] sf::FloatRect Node::getGlobalBounds() const {
	return background.getGlobalBounds();
}