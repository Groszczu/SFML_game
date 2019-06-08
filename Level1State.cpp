#include "Level1State.hpp"
#include "DEFINITIONS.hpp"
#include "SplashState.hpp"
#include "ScoreDisplayState.hpp"
#include "InteractionsHandler.hpp"
#include <thread>
#include "Level2State.hpp"

namespace rstar
{
	Level1State::Level1State(GameDataPtr data)
		: LevelState(data, "Level Background",
			 LVL1_POWERUPS_SPAWN_TIME, LVL1_POINTS)
	{
		player_ = std::make_unique<PlayerShip>(data_, lvlClock_);

		enemies_ = std::make_unique<Enemies>(data_,
			LVL1_ENEMIES_COUNT, LVL1_ENEMIES_MOVEMENT_SPEED, LVL1_ENEMIES_BULLETS_SPEED,
			LVL1_ENEMIES_CHARGING_SPEED, LVL1_ENEMIES_CHARGING_AT_ONCE, LVL1_ENEMIES_LIVES,
			sf::Vector2f{ ENEMIES_SIDE_MARGIN, ENEMIES_TOP_MARGIN }, LVL1_SPACE_BETWEEN_ENEMIES, lvlClock_);
		lvlClock_.restart();
	}

	void Level1State::HandleInput()
	{
		LevelState::HandleInput();
	}

	void Level1State::Update()
	{
		if (fading_)
		{
			updateScore();
			if (playerLives_ <= 0)
			{
				data_->stateMachine.SetState(std::make_unique<ScoreDisplayState>(data_, playerScore_, SCORES_FILEPATH), true);
			}
			else
			{
				data_->stateMachine.SetState(std::make_unique<Level2State>(data_,
					playerLives_, playerScore_, player_->GetMovementSpeed(), player_->GetBulletsSpeed()), true);
			}
		}

		InteractionsHandler::PlayerAndEnemies(*enemies_, *player_, LVL1_POINTS_FOR_ENEMY, LVL1_ENEMIES_CHANCE_TO_SHOOT);
		InteractionsHandler::PlayerAndPowerUp(*player_, powerUpShip_.get());

		LevelState::Update();
	}

	void Level1State::Draw()
	{
		LevelState::Draw();
	}

}
