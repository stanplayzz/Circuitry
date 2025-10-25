#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

class State {
public:
	virtual ~State() = default;

	virtual void enter() {}
	virtual void exit() {}
	virtual void onEvent(sf::Event& event) = 0;
	virtual void update(sf::Time deltaTime) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
};

class StateManager {
private:
	std::vector<std::unique_ptr<State>> states;
public:
	void pushState(std::unique_ptr<State> state);
	void popState(std::unique_ptr<State> state);
	void switchState(std::unique_ptr<State> state);

	void onEvent(sf::Event& event);
	void update(sf::Time deltaTime);
	void draw(sf::RenderWindow& window);
};