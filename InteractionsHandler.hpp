#pragma once
#include "PlayerShip.hpp"
#include "PowerUpShip.hpp"

namespace rstar
{
	class InteractionsHandler
	{
	public:
		static void PlayerAndEnemies(Enemies &e, PlayerShip &ship, unsigned pointsForEnemy, float enemiesChanceToShoot);
		static void PlayerAndPowerUp(PlayerShip &ship, PowerUpShip* powerUp);

	private:

		static void bodiesIntersection(Enemies &e, PlayerShip &ship, unsigned pointsForEnemy);
		static void bullets(Enemies &e, PlayerShip &ship, unsigned pointsForEnemy);
		static void enemiesShooting(Enemies &e, PlayerShip &ship, float chanceToShoot);

		static void powerUpHit(PlayerShip &ship, PowerUpShip* powerUp);
	};
}
