#pragma once

#include "Game.hpp"
#include "PlayerShip.hpp"
#include "LevelState.hpp"

namespace rstar
{
	class Level2State final : public LevelState
	{
	public:
		Level2State(GameDataPtr data, unsigned playerLives, int playerScore, float playerMovementSpeed, float playerBulletsSpeed);

		void HandleInput() override;
		void Update() override;
		void Draw() override;
	};
}


