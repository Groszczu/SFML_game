#pragma once
#include "PlayerShip.hpp"
#include "Enemy.hpp"

namespace rstar
{
	class InteractionsHandler
	{
	public:
		InteractionsHandler() = delete;
		InteractionsHandler(PlayerShip &playerShip, Enemies &enemies);
		void Run();
	private:
		PlayerShip& shipRef_;
		Enemies& enemiesRef_;

		void spritesBodies();
		void playerBullets();
		void enemiesBullets();
	};
}