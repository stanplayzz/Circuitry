#include "nodeManager.hpp"

#include <print>

NodeManager::NodeManager() {
	temp.loadFromFile(ASSETS_DIR + std::string("/textures/machine.png"));

	Node node(sf::Vector2f(512.f, 512.f), temp, 1, 2);
	nodes.push_back(node);

	Node nodey(sf::Vector2f(512.f, 512.f), temp, 1, 1);
	nodes.push_back(nodey);
	
}

void Connection::draw(sf::RenderWindow& window) {
    line.clear();
    sf::Color color(240, 155, 19);

    sf::Vector2f from = fromNode->getPosition() + fromPort->offset;
    sf::Vector2f to = toNode->getPosition() + toPort->offset;
	float spacing = 32.f;
	float dir = (to.x > from.x) ? 1.f : -1.f;

	sf::Vector2f pos1 = { from.x + dir * spacing, from.y };
	sf::Vector2f pos2 = { pos1.x, to.y };

    line.append({ from, color });
	line.append({ pos1, color });
	line.append({ pos2, color });
	line.append({ to, color });

    window.draw(line);
}


void NodeManager::createConnection(Node* nA, Node* nB, Port* pA, Port* pB) {
	connections.push_back(Connection(nA, nB, pA, pB));
}