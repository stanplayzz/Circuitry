#include "nodes/types/producer.hpp"

class IronMine : public Producer {
public:
	IronMine(sf::Texture& texture, NodeManager& nodeManager) 
		: Producer(texture, nodeManager, Resource::Iron, 0, 1) {
		generationRate = 30;
		outputRate = 20;
		capacity = 500;
		nameText.setString("Iron mine");
	}
private:

};