#pragma once
#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

namespace rstar
{
	// pure virtual generic GameObject class
	// contains sprite and virtual functions:
	// --void Update() = 0
	// --void Draw() = 0
	class GameObject
	{
	protected:
		explicit GameObject(GameDataPtr data);
		virtual ~GameObject() = default;

		// sets new GameObject's sprite texture
		void SetTexture(sf::Texture const& texture);

		virtual void Update() = 0;
		virtual void Draw() const = 0;

	public:
		sf::FloatRect GetBounds() const { return sprite_.getGlobalBounds(); }
		sf::Vector2f GetPosition() const { return sprite_.getPosition(); }
		bool IsOutOfScreen() const;

	protected:
		sf::Sprite sprite_;
		GameDataPtr data_;
	};
}