#include "nodes/types/converter.hpp"

class Smelter : public Converter {
public:
	Smelter(sf::Texture& texture, NodeManager& nodeManager)
		: Converter(texture, nodeManager, Resource::Iron, 2, 1) {
		outputRate = 1;
		capacity = 500;
		nameText.setString("Smelter");
	}
private:

};