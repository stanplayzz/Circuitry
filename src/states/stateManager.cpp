#include "stateManager.hpp"

void StateManager::pushState(std::unique_ptr<State> state) {
	if (!states.empty()) states.back()->exit();
	state->enter();
	states.push_back(std::move(state));
}

void StateManager::popState(std::unique_ptr<State> state) {
	if (states.empty()) return;
	states.back()->exit();
	states.pop_back();
	if (!states.empty()) states.back()->enter();
}

void StateManager::switchState(std::unique_ptr<State> state) {
	while (!states.empty()) {
		states.back()->exit();
		states.pop_back();
	}
	state->enter();
	states.push_back(std::move(state));
}

void StateManager::onEvent(sf::Event& event) {
	if (!states.empty()) states.back()->onEvent(event);
}

void StateManager::update(sf::Time deltaTime) {
	if (!states.empty()) states.back()->update(deltaTime);
}

void StateManager::draw(sf::RenderWindow& window) {
	if (!states.empty()) states.back()->draw(window);
}