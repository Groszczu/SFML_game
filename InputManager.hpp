#pragma once
#include <SFML/Graphics.hpp>

namespace rstar
{
	// class that contains public functions checking for objects interaction
	class InputManager
	{
	public:
		bool IsSpriteClicked(sf::Sprite const& object, sf::Mouse::Button const& button, sf::RenderWindow const& window) const;
		bool IsMouseOverSprite(sf::Sprite const& object, sf::RenderWindow const& window) const;
		sf::Vector2i GetMousePosition(sf::RenderWindow const& window) const;
	};
}
