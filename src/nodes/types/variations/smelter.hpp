#include "nodes/types/converter.hpp"

class Smelter : public Converter {
public:
	Smelter(NodeManager& nodeManager)
		: Converter(nodeManager, Resource::Iron, 2, 1) {
		outputRate = 1;
		capacity = 500;
		titleText.setString("Smelter");
		nodeType = NodeType::Smelter;
	}
private:

};