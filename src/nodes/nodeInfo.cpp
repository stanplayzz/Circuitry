#include "nodes/nodeInfo.hpp"
#include "world.hpp"

#include <print>

void NodeInfo::onEvent(sf::Event& event, World& world) {
	if (auto mouse = event.getIf<sf::Event::MouseMoved>()) {
		for (auto& node : world.nodeManager.nodes) {
			if (node->inWorld && node->contains(sf::Vector2f(mouse->position))) {
			}
		}
	}
}