#include "node.hpp"

class NodeManager;

struct Connection {
	Node* fromNode;
	Node* toNode;
	Port* fromPort;
	Port* toPort;
	sf::VertexArray line;

	Connection(Node* f, Node* t,Port* fPort, Port* tPort)
		: fromNode(f), toNode(t), fromPort(fPort), toPort(tPort) {
		line.setPrimitiveType(sf::PrimitiveType::LineStrip);
		line.resize(4);
	}

	void draw(sf::RenderWindow& window);
};


class NodeManager {
public:
	NodeManager();
	void createConnection(Node* nA, Node* nB, Port* pA, Port* pB);

	std::vector<Node> nodes;
	std::vector<Connection> connections;
private:
	sf::Texture temp;

};