#pragma once
#include "stateManager.hpp"
#include "world.hpp"

class Game;

class GameState : public State {
public:
	GameState(Game& game);
	void onEvent(sf::Event& event) override;
	void update(sf::Time deltaTime) override;
	void draw(sf::RenderWindow& window) override;
private:
	World world;
	Game& game;
};