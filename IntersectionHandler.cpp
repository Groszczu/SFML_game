#include "InteractionsHandler.hpp"

namespace rstar
{
	InteractionsHandler::InteractionsHandler(PlayerShip& playerShip, Enemies& enemies)
		: shipRef_(playerShip), enemiesRef_(enemies) {}

	void InteractionsHandler::Run()
	{
		playerBullets();
		spritesBodies();
		enemiesBullets();
	}

	In
}
