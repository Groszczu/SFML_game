#pragma once
#include <SFML/Graphics.hpp>

namespace rstar
{
	// pure virtual class
	// all states has to derive from this class
	// contains Fading and FadingAway static functions
	class State
	{
	public:
		virtual ~State() = default;

		//pure virtual function
		virtual void HandleInput() = 0;
		//pure virtual function
		virtual void Update() = 0;
		//pure virtual function
		virtual void Draw() = 0;

		// static function that smoothly turns screen to the dark
		static void Fading(float time, sf::RenderWindow &window);

		// static function that smoothly makes screen visible again
		static void FadingAway(float time, sf::RenderWindow &window, sf::Sprite const& back);
	};
}
