#pragma once
#include "resources.hpp"

#include <SFML/Graphics.hpp>
#include <UltimateShapes/shapes.hpp>


class Node;
class NodeManager;
enum struct NodeType;

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
	bool dragging = false;
	sf::Vector2f size{128.f, 192.f};
	std::vector<Port> ports;

	Node(NodeManager& nodeManager, const int iPorts = 0, const int oPorts = 0 );
	void setPosition(sf::Vector2f position);
	sf::Vector2f getPosition() const;
	sf::FloatRect getGlobalBounds() const;
	bool contains(sf::Vector2f position) const;
	virtual void update(sf::Time deltaTime) = 0;
	void draw(sf::RenderWindow& window);

	virtual bool recieve(Resource& resource, int amount) = 0;
protected:
	us::SelectiveRoundedRectangleShape titleBackground{};
	us::RoundedRectangleShape background{};
	sf::Texture backgroundTexture;

	NodeManager& nodeManager;
	sf::Texture texture;
	sf::Sprite image;
	sf::Text titleText;
private:
};