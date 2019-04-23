#pragma once
#include <SFML/Graphics.hpp>
#include <random>

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

	template<typename T>
	T Random(T leftRange, T rightRange)
	{
		std::default_random_engine engine{ std::random_device{}() };

		using dist = std::conditional_t<
			std::is_integral<T>::value,
			std::uniform_int_distribution<T>,
			std::uniform_real_distribution<T>
		>;

		return dist{ leftRange, rightRange }(engine);
	}
}
