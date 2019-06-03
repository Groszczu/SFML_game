#pragma once
#include "PowerUpShip.hpp"

namespace rstar
{
	class SpeedPowerUp : public PowerUpShip
	{
	public:
		SpeedPowerUp(GameDataPtr data, float movementSpeed, DirectionX startMoveDirection,  unsigned speed,
			std::vector<sf::Texture> textures, float frameTime, sf::Clock const& clock);

		void PowerUp(PlayerShip& ship) override;
		void Update() override;
		void Draw() const override;
	};
}
