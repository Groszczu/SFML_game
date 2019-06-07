#include "Level2State.hpp"
#include "ScoreDisplayState.hpp"
#include "InteractionsHandler.hpp"
#include "Level3State.hpp"

namespace rstar
{
	Level2State::Level2State(GameDataPtr data, unsigned playerLives, int playerScore, float playerMovementSpeed, float playerBulletsSpeed)
		: LevelState(data, "Level Background",
			LVL2_ENEMIES_COUNT, LVL2_ENEMIES_MOVEMENT_SPEED, LVL2_ENEMIES_BULLETS_SPEED,
			LVL2_ENEMIES_CHARGING_SPEED, LVL2_ENEMIES_CHARGING_AT_ONCE, LVL2_ENEMIES_LIVES,
			sf::Vector2f{ ENEMIES_SIDE_MARGIN, ENEMIES_TOP_MARGIN }, LVL2_SPACE_BETWEEN_ENEMIES, LVL2_POWERUPS_SPAWN_TIME,
			LVL2_POINTS)
	{
		player_ = std::make_unique<PlayerShip>(data_, lvlClock_, playerLives, playerScore, playerMovementSpeed, playerBulletsSpeed);
		powerUpShip_ = nullptr;

		lvlClock_.restart();
	}

	void Level2State::HandleInput()
	{
		LevelState::HandleInput();
	}

	void Level2State::Update()
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
				data_->stateMachine.SetState(std::make_unique<Level3State>(data_,
				playerLives_, playerScore_, player_->GetMovementSpeed(), player_->GetBulletsSpeed()), true);
			}
		}

		InteractionsHandler::PlayerAndEnemies(*enemies_, *player_, LVL2_POINTS_FOR_ENEMY, LVL2_ENEMIES_CHANCE_TO_SHOOT);
		InteractionsHandler::PlayerAndPowerUp(*player_, powerUpShip_.get());

		LevelState::Update();
	}
	
	void Level2State::Draw()
	{
		LevelState::Draw();
	}
}
