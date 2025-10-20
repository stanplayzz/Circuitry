#pragma once

#include <SFML/Graphics.hpp>

class World;

class NodeInfo {
public:
	void onEvent(sf::Event& event, World& world);
private:

};