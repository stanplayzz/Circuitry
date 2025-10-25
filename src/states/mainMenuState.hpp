#pragma once
#include "stateManager.hpp"

#include <UltimateShapes/shapes.hpp>

class Game;

namespace MainMenu {
	struct Button {
		us::RoundedRectangleShape shape;
		sf::Vector2f position{};
		sf::Vector2f targetPos{};
		sf::FloatRect hitBox;

		void setSize(sf::Vector2f size);
		void setPosition(sf::Vector2f position);
		sf::Vector2f getSize() const;
		void draw(sf::RenderWindow& window);
	};

	class MainMenuState : public State {
	public:
		MainMenuState(Game& game);
		void onEvent(sf::Event& event) override;
		void update(sf::Time deltaTime) override;
		void draw(sf::RenderWindow& window) override;
	private:
		Game& game;
		sf::Texture backgroundTexture;
		sf::Texture playTexture;
		sf::Texture quitTexture;

		sf::RectangleShape backgroundShape;
		sf::Font TitanOne_font;
		sf::Font Gruppo_font;
		sf::Text title{ TitanOne_font };
		Button playButton;
		Button quitButton;
	};
}
