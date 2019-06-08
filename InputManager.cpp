#include "InputManager.hpp"
#include "Enemy.hpp"

namespace rstar
{
	bool InputManager::IsSpriteClicked(sf::Sprite const &object, sf::Mouse::Button const &button, sf::RenderWindow const &window) const
	{
		if (sf::Mouse::isButtonPressed(button))
		{
			sf::IntRect tempRect{ static_cast<sf::IntRect>(object.getGlobalBounds()) };

			return tempRect.contains(GetMousePosition(window));
		}

		return false;
	}

	bool InputManager::IsMouseOverSprite(sf::Sprite const& object, sf::RenderWindow const& window) const
	{
		sf::IntRect tempRect{ static_cast<sf::IntRect>(object.getGlobalBounds()) };

		return tempRect.contains(GetMousePosition(window));
	}

	sf::Vector2i InputManager::GetMousePosition(sf::RenderWindow const &window) const
	{
		return sf::Mouse::getPosition(window);
	}

	bool InputManager::IsEnterKey(sf::Uint32 keyCode)
	{
		return keyCode == 13;
	}
	bool InputManager::IsBackspaceKey(sf::Uint32 keyCode)
	{
		return keyCode == 8;
	}
	bool InputManager::IsAlphaCharacter(sf::Uint32 keyCode)
	{
		return keyCode >= 65 && keyCode <= 122;
	}
}