#pragma once
#include "PlayerShip.hpp"
#include "PowerUpShip.hpp"
#include "Boss.hpp"

namespace rstar
{
	class InteractionsHandler
	{
	public:
		static void PlayerAndEnemies(Enemies &e, PlayerShip &ship, unsigned pointsForEnemy, float enemiesChanceToShoot);
		static void PlayerAndPowerUp(PlayerShip &ship, PowerUpShip* powerUp);
		static void PlayerAndBoss(PlayerShip &ship, Boss *boss, unsigned pointsForBoss);

	private:

		static void bodiesIntersection(Enemies &e, PlayerShip &ship, unsigned pointsForEnemy);
		static void bodiesIntersection(PlayerShip &ship, Boss *boss);
		static void bullets(Enemies &e, PlayerShip &ship, unsigned pointsForEnemy);
		static void bullets(PlayerShip &ship, Boss *boss, unsigned pointsForBoss);
		static void enemiesShooting(Enemies &e, PlayerShip &ship, float chanceToShoot);

		static void powerUpHit(PlayerShip &ship, PowerUpShip* powerUp);
	};
}
