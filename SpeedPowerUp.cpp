#include "SpeedPowerUp.hpp"

namespace  rstar
{
	SpeedPowerUp::SpeedPowerUp(GameDataPtr data, float movementSpeed, DirectionX startMoveDirection, unsigned speed,
		std::vector<sf::Texture> textures, float frameTime, sf::Clock const& clock)
		: PowerUpShip(data, movementSpeed, startMoveDirection, speed, textures, frameTime, clock)
	{
	}

	void SpeedPowerUp::PowerUp(PlayerShip& ship)
	{
		ship.IncreaseSpeed(powerUpValue_);
	}

	void SpeedPowerUp::Update()
	{
		PowerUpShip::Update();
	}

	void SpeedPowerUp::Draw() const
	{
		PowerUpShip::Draw();
	}


}
