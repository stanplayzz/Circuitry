#include "nodes/nodeManager.hpp"
#include "nodes/variations/converter.hpp"
#include "nodes/variations/producer.hpp"

#include <print>

NodeManager::NodeManager() {
	temp.loadFromFile(ASSETS_DIR + std::string("/textures/machine.png"));
	font.openFromFile(ASSETS_DIR + std::string("/fonts/Roboto.ttf"));
	nodes.push_back(std::make_unique<Converter>(temp, *this));
	nodes.push_back(std::make_unique<Producer>(temp, *this));
}

void NodeManager::createConnection(Node* nA, Node* nB, Port* pA, Port* pB) {
	Node* nodeA = nA;
	Node* nodeB = nB;
	Port* portA = pA;
	Port* portB = pB;
	
	if (pA->isInput) {
		nodeA = nB;
		nodeB = nA;
		portA = pB;
		portB = pA;
	}
	connections.erase(
		std::remove_if(
			connections.begin(), connections.end(),
			[&](Connection& conn) {
				return conn.fromPort == pA || conn.fromPort == pB
					|| conn.toPort == pA   || conn.toPort == pB;}),
			connections.end());

	Connection c(nodeA, nodeB, portA, portB);
	c.reConnect();
	connections.push_back(c);
}

void Connection::reConnect() {
	line.clear();
	sf::Color color(240, 155, 19);

	sf::Vector2f from = fromNode->getPosition() + fromPort->offset;
	sf::Vector2f to = toNode->getPosition() + toPort->offset;
	auto spacing = 16.f;
	auto dirFrom = !fromPort->isInput ? 1.f : -1.f;
	auto dirTo = toPort->isInput ? 1.f : -1.f;

	sf::Vector2f pos1(from.x + dirFrom * spacing, from.y);
	sf::Vector2f pos2(pos1.x, to.y);
	sf::Vector2f finalPos(to.x - spacing * dirTo, to.y);

	line.append({ from, color });
	line.append({ pos1, color });

	// path generating logic
	if (pos2.x > to.x) {
		pos2.y = toNode->getPosition().y - spacing;
		sf::Vector2f pos3(to.x - spacing, pos2.y);

		if (pos2.y < fromNode->getPosition().y + fromNode->size.y + spacing) {

			if (toNode->getPosition().y <= from.y) {
				pos2.y = fromNode->getPosition().y - spacing;
				if (toNode->getPosition().y - spacing < pos2.y)
					pos2.y = toNode->getPosition().y - spacing;

				if (pos2.y < toNode->getPosition().y + toNode->size.y + spacing
					&& toNode->getPosition().y + toNode->size.y < fromNode->getPosition().y - spacing)
					pos2.y = toNode->getPosition().y + toNode->size.y + spacing;

				pos3 = { finalPos.x, pos2.y };
				line.append({ pos2, color });
				line.append({ pos3, color });
			}
			else {
				pos2.y = fromNode->getPosition().y + fromNode->size.y + spacing;
				if (toNode->getPosition().y + fromNode->size.y + spacing > pos2.y) {
					pos2.y = toNode->getPosition().y + fromNode->size.y + spacing;
				}
				pos3.y = pos2.y;
				line.append({ pos2, color });
				line.append({ pos3, color });
			}
		}
		else {
			line.append({ pos2, color });
			line.append({ pos3, color });
		}
	}
	else { // simple Z
		line.append({ pos2, color });
	}

	line.append({ finalPos, color });
	line.append({ to, color });
}

void Connection::draw(sf::RenderWindow& window) {
    window.draw(line);
}

void NodeManager::updateNode(Node* node) {
	for (auto& connection : connections) {
		if (connection.fromNode == node || connection.toNode == node) {
			connection.reConnect();
		}
	}
}

std::vector<Connection*> NodeManager::getNodeConnections(Node* node) const {
	std::vector<Connection*> res;
	for (auto& connection : connections) {
		if (connection.fromNode == node || connection.toNode == node) {
			res.push_back(const_cast<Connection*>(&connection));
		}
	}
	return res;
}

void NodeManager::removeConnection(Connection* connection) {
	connections.erase(std::remove_if(
		connections.begin(), connections.end(),
		[&](const Connection& conn) {
			return &conn == connection;
		}),
		connections.end());
}

void NodeManager::update(sf::Time deltaTime) {
	for (auto& node : nodes) {
		node->update(deltaTime);
	}
}