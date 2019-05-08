#pragma once
#include "PlayerShip.hpp"
#include "Enemy.hpp"

namespace rstar
{
	class InteractionsHandler
	{
	public:
		void Run(Enemies &e, PlayerShip &ship);
	private:

		void bodiesIntersection(Enemies &e, PlayerShip &ship);
		void bullets(Enemies &e, PlayerShip &ship);
		void enemiesShooting(Enemies &e, PlayerShip &ship);
	};
}