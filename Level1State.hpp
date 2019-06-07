#pragma once
#include "Game.hpp"
#include "PlayerShip.hpp"
#include "LevelState.hpp"

namespace rstar
{
	class Level1State final : public LevelState
	{
	public:
		explicit Level1State(GameDataPtr data);

		void HandleInput() override;
		void Update() override;
		void Draw() override;
	};
}

