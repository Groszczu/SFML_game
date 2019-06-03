#pragma once
#include "PowerUpShip.hpp"

namespace rstar
{
	class LivesPowerUp : public PowerUpShip
	{
	public:
		LivesPowerUp(GameDataPtr data, float movementSpeed, DirectionX startMoveDirection, unsigned lives, 
			std::vector<sf::Texture> const& textures, float frameTime, sf::Clock const& clock);

		void PowerUp(PlayerShip& ship) override;
		void Update() override;
		void Draw() const override;
	};
}

