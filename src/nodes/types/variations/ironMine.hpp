#include "nodes/types/producer.hpp"

class IronMine : public Producer {
public:
	
	IronMine(sf::Texture& texture, NodeManager& nodeManager) 
		: Producer(texture, nodeManager, 0, 1) {
		resource = Resource::Iron;
		generationRate = 2;
		outputRate = 3;
		capacity = 20;
	}
private:

};