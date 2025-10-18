#include "nodes/node.hpp"

class NodeManager;

struct Connection {
	Node* fromNode;
	Node* toNode;
	Port* fromPort;
	Port* toPort;
	sf::VertexArray line{ sf::PrimitiveType::LineStrip };

	Connection(Node* f, Node* t,Port* fPort, Port* tPort)
		: fromNode(f), toNode(t), fromPort(fPort), toPort(tPort) {
	}

	void reConnect();
	void draw(sf::RenderWindow& window);
};


class NodeManager {
public:
	NodeManager();
	void createConnection(Node* nA, Node* nB, Port* pA, Port* pB);
	void removeConnection(Connection* connection);
	void update(sf::Time deltaTime);
	void updateNode(Node* node);
	std::vector<Connection*> getNodeConnections(Node* node) const;

	std::vector<std::unique_ptr<Node>> nodes;
	std::vector<Connection> connections;
	sf::Font font;
private:
	sf::Texture temp;

};