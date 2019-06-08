#pragma once
#include "LevelState.hpp"

namespace rstar
{
	class Level3State final : public LevelState
	{
	public:
		Level3State(GameDataPtr data, unsigned playerLives, int playerScore, float playerMovementSpeed, float playerBulletsSpeed);

		void HandleInput() override;
		void Update() override;
		void Draw() override;
	};
}
