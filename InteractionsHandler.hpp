#pragma once
#include "PlayerShip.hpp"
#include "Enemy.hpp"

namespace rstar
{
	class InteractionsHandler
	{
	public:
		static void Run(Enemies &e, PlayerShip &ship);
	private:

		static void bodiesIntersection(Enemies &e, PlayerShip &ship);
		static void bullets(Enemies &e, PlayerShip &ship);
		static void enemiesShooting(Enemies &e, PlayerShip &ship);
	};
}