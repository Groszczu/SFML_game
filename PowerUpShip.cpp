#include "PowerUpShip.hpp"


namespace rstar
{
	PowerUpShip::PowerUpShip(GameDataPtr data, float movementSpeed, Direction startMoveDirection, sf::Clock &clock)
		: GameObject(data), movementSpeed_(movementSpeed), moveDirection_(startMoveDirection), clockRef_(clock) {}
}
