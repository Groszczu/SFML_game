#include "Level1State.hpp"
#include "DEFINITIONS.hpp"
#include "Enemy.hpp"
#include "SplashState.hpp"
#include "ScoreDisplayState.hpp"
#include "InteractionsHandler.hpp"
#include <thread>
#include "Level2State.hpp"
#include "LivesPowerUp.hpp"
#include "BulletsSpeedPowerUp.hpp"
#include "SpeedPowerUp.hpp"

namespace rstar
{
	Level1State::Level1State(GameDataPtr data)
		: data_{ std::move(data) }
	{
		data_->assets.LoadTexture("Level Background", { LEVEL_BACKGROUND1_FILEPATH, LEVEL_BACKGROUND2_FILEPATH, LEVEL_BACKGROUND3_FILEPATH });

		data_->assets.LoadTexture("Player Ship", { PLAYER_SHIP_FILEPATH });
		data_->assets.LoadTexture("Hit Player Ship", { HIT_PLAYER_SHIP_FILEPATH });
		data_->assets.LoadTexture("Player Bullet", { PLAYER_BULLET_FILEPATH });
		data_->assets.LoadTexture("Enemy Bullet", { ENEMY_BULLET_FILEPATH });

		data_->assets.LoadTexture("Red Enemy", { RED_ENEMY1_FILEPATH, RED_ENEMY2_FILEPATH, RED_ENEMY3_FILEPATH });

		data_->assets.LoadTexture("Enemy dst", { ENEMY_DST1_FILEPATH, ENEMY_DST2_FILEPATH, ENEMY_DST3_FILEPATH, ENEMY_DST4_FILEPATH });

		data_->assets.LoadTexture("Lives PowerUp", { LIVES_POWERUP1_FILEPATH, LIVES_POWERUP2_FILEPATH, LIVES_POWERUP3_FILEPATH });
		data_->assets.LoadTexture("Speed PowerUp", { SPEED_POWERUP1_FILEPATH, SPEED_POWERUP2_FILEPATH, SPEED_POWERUP3_FILEPATH });
		data_->assets.LoadTexture("Bullets Speed PowerUp", { BULLETS_POWERUP1_FILEPATH, BULLETS_POWERUP2_FILEPATH, BULLETS_POWERUP3_FILEPATH });

		data_->assets.LoadFont("Pixel Font", PIXEL_FONT_FILEPATH);

		background_.setTexture(data_->assets.GetTexture("Level Background"));

		scoreTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		scoreTxt_.setCharacterSize(IN_GAME_FONT_SIZE);
		scoreTxt_.setPosition(0, WINDOW_HEIGHT - IN_GAME_FONT_SIZE);
		scoreTxt_.setString("SCORE: " + std::to_string(playerScore_));

		playerLivesTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		playerLivesTxt_.setCharacterSize(IN_GAME_FONT_SIZE);
		playerLivesTxt_.setPosition(WINDOW_WIDTH - 10 * IN_GAME_FONT_SIZE, WINDOW_HEIGHT - IN_GAME_FONT_SIZE);
		playerLivesTxt_.setString("LIVES: " + std::to_string(playerLives_));

		player_ = std::make_unique<PlayerShip>(data_, lvlClock_);

		enemies_ = std::make_unique<Enemies>(data_, LVL1_ENEMIES_COUNT, LVL1_ENEMIES_MOVEMENT_SPEED, LVL1_ENEMIES_BULLETS_SPEED, LVL1_ENEMIES_CHARGING_SPEED,
			LVL1_ENEMIES_CHARGING_AT_ONCE, sf::Vector2f{ ENEMIES_SIDE_MARGIN, ENEMIES_TOP_MARGIN }, LVL1_SPACE_BETWEEN_ENEMIES, lvlClock_);

		powerUpShip_ = nullptr;

		backgroundThread_ = std::thread(&Level1State::backgroundAnimation, this);

		lvlClock_.restart();
	}

	Level1State::~Level1State()
	{
		stopThread_ = true;
		if (backgroundThread_.joinable())
		{
			backgroundThread_.join();
		}
	}

	void Level1State::HandleInput()
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

	void Level1State::Draw()
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

	void Level1State::backgroundAnimation()
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

	void Level1State::updateScore()
	{
		if (fading_ && lvlCompleteTime_ > 0)
		{
			playerScore_ += static_cast<int>(LVL1_POINTS / lvlCompleteTime_);
		}
		else if (player_->GetScore() != playerScore_)
		{
			playerScore_ = player_->GetScore();
			scoreTxt_.setString("SCORE: " + std::to_string(playerScore_));
		}
	}

	void Level1State::updateLives()
	{
		if (player_->GetLives() != playerLives_ && player_->GetLives() >= 0)
		{
			playerLives_ = player_->GetLives();
			playerLivesTxt_.setString("LIVES: " + std::to_string(playerLives_));
		}
	}

	void Level1State::handlePowerUpsSpawning()
	{
		if (lvlClock_.getElapsedTime().asSeconds() - powerUpsSpawnTimeOffset > LVL1_POWERUPS_SPAWN_TIME)
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
