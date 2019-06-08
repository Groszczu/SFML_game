#pragma once
#include "GameObject.hpp"
#include "Game.hpp"
#include "DEFINITIONS.hpp"

namespace rstar
{
	class Bullet : public GameObject
	{
	public:
		Bullet(GameDataPtr data, sf::Texture const&, sf::Vector2f startPosition,
			float movementSpeed, DirectionX directionX = DirectionX::none, DirectionY directionY = DirectionY::down);

		void Update() override;
		void Draw() const override;

	private:
		float movementSpeed_;
		DirectionX directionX_;
		DirectionY directionY_;
	};
}
