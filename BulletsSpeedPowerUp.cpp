#include "BulletsSpeedPowerUp.hpp"
#include "Enemy.hpp"

namespace rstar
{
	BulletsSpeedPowerUp::BulletsSpeedPowerUp(GameDataPtr data, float movementSpeed, DirectionX startMoveDirection, unsigned speed,
		std::vector<sf::Texture> textures, float frameTime, sf::Clock const& clock)
		: PowerUpShip(data, movementSpeed, startMoveDirection, speed, textures, frameTime, clock)
	{
	}

	void BulletsSpeedPowerUp::PowerUp(PlayerShip& ship)
	{
		ship.IncreaseBulletsSpeed(powerUpValue_);
	}

	void BulletsSpeedPowerUp::Update()
	{
		PowerUpShip::Update();
	}

	void BulletsSpeedPowerUp::Draw() const
	{
		PowerUpShip::Draw();
	}
}
