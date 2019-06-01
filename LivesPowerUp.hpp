#pragma once
#include "PowerUpShip.hpp"

namespace rstar
{
	class LivesPowerUp : public PowerUpShip
	{
	public:
		LivesPowerUp(GameDataPtr data, float movementSpeed, Direction startMoveDirection, unsigned lives, sf::Clock &clock);

		void PowerUp(PlayerShip& ship) override;
		void Update() override;
		void Draw() const override;

	private:
		unsigned lives_;

		void animate();
		void animateDestroy();
	};
}

