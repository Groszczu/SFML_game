#include "LivesPowerUp.hpp"

namespace rstar
{
	LivesPowerUp::LivesPowerUp(GameDataPtr data, float movementSpeed, DirectionX startMoveDirection, unsigned lives, 
		std::vector<sf::Texture> textures, float frameTime, sf::Clock const& clock)
		: PowerUpShip(data, movementSpeed, startMoveDirection, lives, textures, frameTime, clock)
	{
	}

	void LivesPowerUp::PowerUp(PlayerShip& ship)
	{
		ship.IncreaseLives(powerUpValue_);
	}

	void LivesPowerUp::Update()
	{
		PowerUpShip::Update();
	}

	void LivesPowerUp::Draw() const
	{
		PowerUpShip::Draw();
	}


}
