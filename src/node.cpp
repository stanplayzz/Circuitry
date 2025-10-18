#include "node.hpp"

Node::Node(sf::Vector2f position, sf::Texture& texture, const int iPorts, const int oPorts) : image(texture) {
	background.setSize(size);
	background.setFillColor(sf::Color(40, 40, 40));
	image.setScale({ 0.2f, 0.2f });

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

	setPosition(position);
}

void Node::setPosition(sf::Vector2f position) {
	background.setPosition(position);
	auto offset = (size.x - 512.f * image.getScale().x) / 2.f;
	image.setPosition(position + sf::Vector2f(offset, offset));

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
	window.draw(image);
	for (auto& port : ports)
		window.draw(port.shape);
}

[[nodiscard]] bool Port::contains(sf::Vector2f position) const {
	return shape.getGlobalBounds().contains(position);
}

[[nodiscard]] sf::FloatRect Node::getGlobalBounds() const {
	return background.getGlobalBounds();
}