#pragma once
#include "PowerUpShip.hpp"

namespace rstar
{
	class BulletsSpeedPowerUp : public PowerUpShip
	{
	public:
		BulletsSpeedPowerUp(GameDataPtr data, float movementSpeed, DirectionX startMoveDirection, unsigned speed, 
			std::vector<sf::Texture> textures, float frameTime, sf::Clock const& clock);

		void PowerUp(PlayerShip& ship) override;
		void Update() override;
		void Draw() const override;
	};
}
