#pragma once
#include "PlayerShip.hpp"

namespace rstar
{
	class InteractionsHandler
	{
	public:
		static void Run(Enemies &e, PlayerShip &ship, unsigned pointsForEnemy, float enemiesChanceToShoot);
	private:

		static void bodiesIntersection(Enemies &e, PlayerShip &ship, unsigned pointsForEnemy);
		static void bullets(Enemies &e, PlayerShip &ship, unsigned pointsForEnemy);
		static void enemiesShooting(Enemies &e, PlayerShip &ship, float chanceToShoot);
	};
}