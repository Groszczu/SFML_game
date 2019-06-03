#include "Level2State.hpp"
#include "ScoreDisplayState.hpp"
#include "InteractionsHandler.hpp"
#include "LivesPowerUp.hpp"
#include "SpeedPowerUp.hpp"
#include "BulletsSpeedPowerUp.hpp"

namespace rstar
{
	Level2State::Level2State(GameDataPtr data, int playerLives, int playerScore, float playerMovementSpeed, float playerBulletsSpeed)
		: data_(std::move(data)), playerLives_(playerLives), playerScore_(playerScore),
		player_(std::make_unique<PlayerShip>(data_, lvlClock_, playerLives_, playerScore_, playerMovementSpeed, playerBulletsSpeed)) 
	{
		background_.setTexture(data_->assets.GetTexture("Level Background"));

		scoreTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		scoreTxt_.setCharacterSize(IN_GAME_FONT_SIZE);
		scoreTxt_.setPosition(0, WINDOW_HEIGHT - IN_GAME_FONT_SIZE);
		scoreTxt_.setString("SCORE: " + std::to_string(playerScore_));

		playerLivesTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		playerLivesTxt_.setCharacterSize(IN_GAME_FONT_SIZE);
		playerLivesTxt_.setPosition(WINDOW_WIDTH - 10 * IN_GAME_FONT_SIZE, WINDOW_HEIGHT - IN_GAME_FONT_SIZE);
		playerLivesTxt_.setString("LIVES: " + std::to_string(playerLives_));

		enemies_ = std::make_unique<Enemies>(data_, LVL2_ENEMIES_COUNT, LVL2_ENEMIES_MOVEMENT_SPEED, LVL2_ENEMIES_BULLETS_SPEED, LVL2_ENEMIES_CHARGING_SPEED,
			LVL2_ENEMIES_CHARGING_AT_ONCE, sf::Vector2f{ ENEMIES_SIDE_MARGIN, ENEMIES_TOP_MARGIN }, LVL2_SPACE_BETWEEN_ENEMIES, lvlClock_);

		powerUpShip_ = nullptr;

		backgroundThread_ = std::thread(&Level2State::backgroundAnimation, this);

		lvlClock_.restart();
	}

	Level2State::~Level2State()
	{
		stopThread_ = true;
		if (backgroundThread_.joinable())
		{
			backgroundThread_.join();
		}
	}

	void Level2State::HandleInput()
	{
		sf::Event ev{};

		while (data_->window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				data_->window.close();
			}
		}
	}

	void Level2State::Update()
	{

		if (fading_)
		{
			updateScore();
			data_->stateMachine.SetState(std::make_unique<ScoreDisplayState>(data_, playerScore_, SCORES_FILEPATH), true);
		}

		InteractionsHandler::PlayerAndEnemies(*enemies_, *player_, LVL2_POINTS_FOR_ENEMY, LVL2_ENEMIES_CHANCE_TO_SHOOT);
		InteractionsHandler::PlayerAndPowerUp(*player_, powerUpShip_.get());

		player_->Update();
		enemies_->Update();
		if (powerUpShip_)
		{
			if (powerUpShip_->IsToRemove())
			{
				powerUpShip_.reset(nullptr);
			}
			else
			{
				powerUpShip_->Update();
			}
		}
		else
		{
			handlePowerUpsSpawning();
		}

		updateScore();
		updateLives();

		if (player_->GetLives() <= 0)
		{
			// no points for losing lvl
			lvlCompleteTime_ = -1.f;
			fading_ = true;
		}

		if (enemies_->GetEnemiesCount() <= 0 && !powerUpShip_)
		{
			lvlCompleteTime_ = lvlClock_.getElapsedTime().asSeconds();
			fading_ = true;
		}
	}
	
	void Level2State::Draw()
	{
		if (initial_)
		{
			data_->window.draw(background_);
			FadingAway(SHADOW_FRAME_TIME, data_->window, background_);
			initial_ = false;
		}
		else if (fading_)
		{
			Fading(SHADOW_FRAME_TIME, data_->window);
		}
		else
		{
			data_->window.clear();
			data_->window.draw(background_);

			player_->Draw();
			enemies_->Draw();
			if (powerUpShip_) { powerUpShip_->Draw(); }

			data_->window.draw(scoreTxt_);
			data_->window.draw(playerLivesTxt_);

			data_->window.display();
		}
	}

	void Level2State::backgroundAnimation()
	{
		auto localTimeOffset{ 0.f };
		while (!stopThread_)
		{
			if (lvlClock_.getElapsedTime().asSeconds() - localTimeOffset > BACKGROUND_ANIMATION_FRAME_TIME)
			{
				if (backgroundCurrentTexture_ >= data_->assets.GetTexturesArray("Level Background").size())
				{
					backgroundCurrentTexture_ = 0;
				}

				background_.setTexture(data_->assets.GetTexture("Level Background", backgroundCurrentTexture_++));
				localTimeOffset = lvlClock_.getElapsedTime().asSeconds();
			}
		}
	}

	void Level2State::updateScore()
	{
		if (fading_ && lvlCompleteTime_ > 0)
		{
			playerScore_ += static_cast<int>(LVL2_POINTS / lvlCompleteTime_);
		}
		else if (player_->GetScore() != playerScore_)
		{
			playerScore_ = player_->GetScore();
			scoreTxt_.setString("SCORE: " + std::to_string(playerScore_));
		}
	}

	void Level2State::updateLives()
	{
		if (player_->GetLives() != playerLives_ && player_->GetLives() >= 0)
		{
			playerLives_ = player_->GetLives();
			playerLivesTxt_.setString("LIVES: " + std::to_string(playerLives_));
		}
	}

	void Level2State::handlePowerUpsSpawning()
	{
		if (lvlClock_.getElapsedTime().asSeconds() - powerUpsSpawnTimeOffset > LVL2_POWERUPS_SPAWN_TIME)
		{
			auto const powerUpType{ Random<int>(1, 3) };
			auto const powerUpMoveDirection{ Random<int>(0, 1) };

			switch (powerUpType)
			{
			case 1:
				powerUpShip_ = std::make_unique<LivesPowerUp>(data_, POWERUPS_MOVEMENT_SPEED,
					powerUpMoveDirection == 0 ? DirectionX::left : DirectionX::right, LIVES_POWERUP_VALUE,
					data_->assets.GetTexturesArray("Lives PowerUp"), ENEMY_ANIMATION_FRAME_TIME, lvlClock_);
				break;
			case 2:
				powerUpShip_ = std::make_unique<SpeedPowerUp>(data_, POWERUPS_MOVEMENT_SPEED,
					powerUpMoveDirection == 0 ? DirectionX::left : DirectionX::right, SPEED_POWERUP_VALUE,
					data_->assets.GetTexturesArray("Speed PowerUp"), ENEMY_ANIMATION_FRAME_TIME, lvlClock_);
				break;
			case 3:
				powerUpShip_ = std::make_unique<BulletsSpeedPowerUp>(data_, POWERUPS_MOVEMENT_SPEED,
					powerUpMoveDirection == 0 ? DirectionX::left : DirectionX::right, BULLETS_SPEED_POWERUP_VALUE,
					data_->assets.GetTexturesArray("Bullets Speed PowerUp"), ENEMY_ANIMATION_FRAME_TIME, lvlClock_);
				break;
			default:
				break;
			}
			powerUpsSpawnTimeOffset = lvlClock_.getElapsedTime().asSeconds();
		}
	}
}
