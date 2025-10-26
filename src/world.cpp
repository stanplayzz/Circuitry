#include "game.hpp"
#include "world.hpp"
#include "nodes/types/variations/ironMine.hpp"
#include "nodes/types/variations/smelter.hpp"
#include <UltimateShapes/shapes.hpp>

#include <fstream>

auto gridColor = sf::Color(255, 255, 255, 25);

void World ::save() {
	std::ofstream file(ASSETS_DIR + std::string("/save.save"), std::ios::binary);
	if (!file) return;
	auto count = (uint32_t)nodeManager.nodes.size();
	file.write((char*)&count, sizeof(count));
	for (const auto& node : nodeManager.nodes) {
		int type = (int)node->nodeType;
		file.write((char*)&type, sizeof(type));
		sf::Vector2f pos = node->getPosition();
		file.write((char*)&pos, sizeof(pos));
		bool inWorld = node->inWorld;
		file.write((char*)&inWorld, sizeof(inWorld));
	}
	auto connectionCount = (uint32_t)nodeManager.connections.size();
	file.write((char*)&connectionCount, sizeof(connectionCount));
	for (const auto& conn : nodeManager.connections) {
		int fromIndex, toIndex, fromPortIndex, toPortIndex = -1;
		for (size_t i = 0; i < nodeManager.nodes.size(); i++) {
			if (nodeManager.nodes[i].get() == conn.fromNode) fromIndex = (int)i;
			if (nodeManager.nodes[i].get() == conn.toNode) toIndex = (int)i;
		}
		if (conn.fromNode && conn.fromPort) {
			for (size_t i = 0; i < conn.fromNode->ports.size(); i++) {
				if (&conn.fromNode->ports[i] == conn.fromPort) fromPortIndex = (int)i;
			}
		}
		if (conn.toNode && conn.toPort) {
			for (size_t i = 0; i < conn.toNode->ports.size(); i++) {
				if (&conn.toNode->ports[i] == conn.toPort) toPortIndex = (int)i;
			}
		}

		file.write((char*)&fromIndex, sizeof(fromIndex));
		file.write((char*)&toIndex, sizeof(toIndex));
		file.write((char*)&fromPortIndex, sizeof(fromPortIndex));
		file.write((char*)&toPortIndex, sizeof(toPortIndex));
	}
}

void World::load() {
	std::ifstream file(ASSETS_DIR + std::string("/save.save"), std::ios::binary);
	if (!file) return;
	uint32_t count;
	file.read((char*)(&count), sizeof(count));

	nodeManager.nodes.clear();
	for (uint32_t i = 0; i < count; i++) {
		int type;
		sf::Vector2f pos;
		bool inWorld;
		file.read((char*)&type, sizeof(type));
		file.read((char*)&pos, sizeof(pos));
		file.read((char*)&inWorld, sizeof(inWorld));
		std::unique_ptr<Node> node;
		switch ((NodeType)type) {
		case NodeType::IronMine: node = std::make_unique<IronMine>(nodeManager); break;
		case NodeType::Smelter: node = std::make_unique<Smelter>(nodeManager); break;
		}
		if (node) {
			node->setPosition(pos);
			node->inWorld = inWorld;
			nodeManager.nodes.push_back(std::move(node));
		}
	}
	auto connectionCount = (uint32_t)nodeManager.connections.size();
	file.read((char*)&connectionCount, sizeof(connectionCount));
	nodeManager.connections.clear();
	for (uint32_t i = 0; i < connectionCount; i++) {
		int fromIndex, toIndex, fromPortIndex, toPortIndex = -1;
		file.read((char*)&fromIndex, sizeof(fromIndex));
		file.read((char*)&toIndex, sizeof(toIndex));
		file.read((char*)&fromPortIndex, sizeof(fromPortIndex));
		file.read((char*)&toPortIndex, sizeof(toPortIndex));
		if (fromIndex < 0 || toIndex < 0) continue;
		if (fromIndex >= (int)nodeManager.nodes.size() || toIndex >= (int)nodeManager.nodes.size()) continue;

		Node* fromNode = nodeManager.nodes[fromIndex].get();
		Node* toNode = nodeManager.nodes[toIndex].get();
		if (!fromNode || !toNode) continue;

		Port* fromPort = nullptr;
		Port* toPort = nullptr;
		if (fromPortIndex >= 0 && fromPortIndex < (int)fromNode->ports.size()) {
			fromPort = &fromNode->ports[fromPortIndex];
		}
		if (toPortIndex >= 0 && toPortIndex < (int)toNode->ports.size()) {
			toPort = &toNode->ports[toPortIndex];
		}
		if (fromPort && toPort) {
			nodeManager.connections.emplace_back(Connection(fromNode, toNode, fromPort, toPort));
			nodeManager.connections.back().reConnect();
		}
	}
}

World::World(Game& game) {
	playerHandler = std::make_unique<PlayerHandler>(game);
	toolbar = std::make_unique<Toolbar>(game);

	grid.setPrimitiveType(sf::PrimitiveType::Lines);

	for (float x = 0; x <= world_grid_size.x; x++) {
		grid.append(sf::Vertex({ x * tile_size, 0.f }, gridColor));
		grid.append(sf::Vertex({ x * tile_size, (float)world_grid_size.x * tile_size }, gridColor));
	}
	for (float y = 0; y <= world_grid_size.y; y++) {
		grid.append(sf::Vertex({ 0.f, y * tile_size }, gridColor));
		grid.append(sf::Vertex({ (float)world_grid_size.y * tile_size, y * tile_size }, gridColor));
	}

	load();
}

void World::update(sf::RenderWindow& window, sf::Time deltaTime) {
	playerHandler->update(window, nodeManager);
	nodeManager.update(deltaTime,  window);
	toolbar->update(deltaTime, *this);
}

void World::onEvent(sf::Event& event, sf::RenderWindow& window, Game& game, sf::View& worldView) {
	bool shouldUpdateView = false;
	if (auto mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse->button == sf::Mouse::Button::Right) {
			panning = true;
			prevMouse = sf::Mouse::getPosition(window);
		}
	}

	if (auto mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
		if (mouse->button == sf::Mouse::Button::Right) {
			panning = false;
		}
	}
	if (auto scroll = event.getIf<sf::Event::MouseWheelScrolled>()) {
		float zoomStep = (scroll->delta > 0) ? 0.9f : 1.1f;
		float newZoom = currentZoom * zoomStep;

		sf::Vector2f newSize = window.getDefaultView().getSize() * newZoom;
		sf::Vector2f worldSize(world_grid_size.x * tile_size, world_grid_size.y * tile_size);
		if (newSize.x > worldSize.x) newZoom = worldSize.x / window.getDefaultView().getSize().x;
		if (newSize.y > worldSize.y) newZoom = worldSize.y / window.getDefaultView().getSize().y;

		if (newZoom < 0.5f) newZoom = 0.5f;
		currentZoom = newZoom;
		
		worldView.setSize(window.getDefaultView().getSize() * currentZoom);
		shouldUpdateView = true;
	}
	if (panning) {
		auto mouse = sf::Mouse::getPosition(window);
		worldView.move(window.mapPixelToCoords(prevMouse) - window.mapPixelToCoords(mouse));
		prevMouse = mouse;
		shouldUpdateView = true;
	}

	if (shouldUpdateView) {
		auto half = worldView.getSize() / 2.f;
		auto center = worldView.getCenter();

		center.x = std::max(half.x, std::min(center.x, world_grid_size.x * tile_size - half.x));
		center.y = std::max(half.y, std::min(center.y, world_grid_size.y * tile_size - half.y));

		worldView.setCenter(center);
		window.setView(worldView);
	}


	if (auto key = event.getIf<sf::Event::KeyReleased>()) {
		if (key->scancode == sf::Keyboard::Scancode::P) {
			save();
		}
	}

	nodeManager.onEvent(event, window, *this);
	playerHandler->onEvent(event, window, *this);
	toolbar->onEvent(event, window, game);
}

void World::draw(sf::RenderWindow& window, Game& game) {
	window.draw(grid);

	nodeManager.draw(window, *this);

	toolbar->draw(window, game, *this);
	// draw nodes that should be on top of everything
	if (playerHandler->currentNode)
		playerHandler->currentNode->draw(window);
}