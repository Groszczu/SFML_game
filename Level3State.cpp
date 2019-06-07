#include "Level3State.hpp"
#include "ScoreDisplayState.hpp"
#include "InteractionsHandler.hpp"


namespace rstar
{
	Level3State::Level3State(GameDataPtr data, unsigned playerLives, int playerScore, float playerMovementSpeed, float playerBulletsSpeed)
		: LevelState(data, "Level Background",
			LVL3_ENEMIES_COUNT, LVL3_ENEMIES_MOVEMENT_SPEED, LVL3_ENEMIES_BULLETS_SPEED,
			LVL3_ENEMIES_CHARGING_SPEED, LVL3_ENEMIES_CHARGING_AT_ONCE, LVL3_ENEMIES_LIVES,
			sf::Vector2f{ ENEMIES_SIDE_MARGIN, ENEMIES_TOP_MARGIN }, LVL3_SPACE_BETWEEN_ENEMIES, LVL3_POWERUPS_SPAWN_TIME,
			LVL3_POINTS, true)
	{
		player_ = std::make_unique<PlayerShip>(data_, lvlClock_, playerLives, playerScore, playerMovementSpeed, playerBulletsSpeed);
		powerUpShip_ = nullptr;

		lvlClock_.restart();
	}

	void Level3State::HandleInput()
	{
		LevelState::HandleInput();
	}

	void Level3State::Update()
	{
		if (fading_)
		{
			updateScore();
			data_->stateMachine.SetState(std::make_unique<ScoreDisplayState>(data_, playerScore_, SCORES_FILEPATH), true);
		}

		InteractionsHandler::PlayerAndEnemies(*enemies_, *player_, LVL3_POINTS_FOR_ENEMY, LVL3_ENEMIES_CHANCE_TO_SHOOT);
		InteractionsHandler::PlayerAndPowerUp(*player_, powerUpShip_.get());

		LevelState::Update();
	
	}

	void Level3State::Draw()
	{
		LevelState::Draw();
	}
}
