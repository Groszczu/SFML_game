#include "LevelState.hpp"
#include <utility>
#include "LivesPowerUp.hpp"
#include "SpeedPowerUp.hpp"
#include "BulletsSpeedPowerUp.hpp"


namespace rstar
{
	LevelState::LevelState(GameDataPtr data, std::string const& backgroundTextureName, float powerUpsSpawnTime,
		unsigned lvlCompletionPoints)
		: data_(std::move(data)), backgroundTextures_(data_->assets.GetTexturesArray(backgroundTextureName)),
		powerUpsSpawnTime_(powerUpsSpawnTime), lvlCompletePoints_(lvlCompletionPoints)
	{
		data_->assets.LoadFont("Pixel Font", PIXEL_FONT_FILEPATH);

		data_->assets.LoadTexture("Player Ship", { PLAYER_SHIP_FILEPATH });
		data_->assets.LoadTexture("Hit Player Ship", { HIT_PLAYER_SHIP_FILEPATH });

		data_->assets.LoadTexture("Player Bullet", { PLAYER_BULLET_FILEPATH });
		data_->assets.LoadTexture("Enemy Bullet", { ENEMY_BULLET_FILEPATH });
		data_->assets.LoadTexture("Boss Bullet", { BOSS_BULLET_FILEPATH });
		data_->assets.LoadTexture("Boss Laser", { BOSS_LASER_FILEPATH });

		data_->assets.LoadTexture("Red Enemy", { RED_ENEMY1_FILEPATH, RED_ENEMY2_FILEPATH, RED_ENEMY3_FILEPATH });
		data_->assets.LoadTexture("Blue Enemy", { BLUE_ENEMY1_FILEPATH, BLUE_ENEMY2_FILEPATH, BLUE_ENEMY3_FILEPATH });
		data_->assets.LoadTexture("Gold Enemy", { GOLD_ENEMY1_FILEPATH, GOLD_ENEMY2_FILEPATH, GOLD_ENEMY3_FILEPATH });

		data_->assets.LoadTexture("Boss", { BOSS_FILEPATH, BOSS_FILEPATH1, BOSS_FILEPATH2, BOSS_FILEPATH3,
			BOSS_FILEPATH4, BOSS_FILEPATH5, BOSS_FILEPATH6, BOSS_FILEPATH7, BOSS_FILEPATH8, BOSS_FILEPATH9,
			BOSS_FILEPATH10, BOSS_FILEPATH11, BOSS_FILEPATH12, BOSS_FILEPATH13, BOSS_FILEPATH14, BOSS_FILEPATH15 });

		data_->assets.LoadTexture("Enemy dst", { ENEMY_DST1_FILEPATH, ENEMY_DST2_FILEPATH, ENEMY_DST3_FILEPATH, ENEMY_DST4_FILEPATH });

		data_->assets.LoadTexture("Lives PowerUp", { LIVES_POWERUP1_FILEPATH, LIVES_POWERUP2_FILEPATH, LIVES_POWERUP3_FILEPATH });
		data_->assets.LoadTexture("Speed PowerUp", { SPEED_POWERUP1_FILEPATH, SPEED_POWERUP2_FILEPATH, SPEED_POWERUP3_FILEPATH });
		data_->assets.LoadTexture("Bullets Speed PowerUp", { BULLETS_POWERUP1_FILEPATH, BULLETS_POWERUP2_FILEPATH, BULLETS_POWERUP3_FILEPATH });

		scoreTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		scoreTxt_.setCharacterSize(IN_GAME_FONT_SIZE);
		scoreTxt_.setPosition(0, WINDOW_HEIGHT - IN_GAME_FONT_SIZE);
		scoreTxt_.setString("SCORE: " + std::to_string(playerScore_));

		playerLivesTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		playerLivesTxt_.setCharacterSize(IN_GAME_FONT_SIZE);
		playerLivesTxt_.setPosition(WINDOW_WIDTH - 10 * IN_GAME_FONT_SIZE, WINDOW_HEIGHT - IN_GAME_FONT_SIZE);
		playerLivesTxt_.setString("LIVES: " + std::to_string(playerLives_));
	}

	void LevelState::HandleInput()
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

	void LevelState::Update()
	{
		backgroundAnimation();

		player_->Update();
		enemies_->Update();
		if (boss_)
		{
			if (boss_->IsToRemove())
			{
				boss_.reset(nullptr);
			}
			else
			{
				boss_->Update();
			}
		}

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

		if (enemies_->GetEnemiesCount() <= 0 && !powerUpShip_ && !boss_)
		{
			lvlCompleteTime_ = lvlClock_.getElapsedTime().asSeconds();
			fading_ = true;
		}
	}

	void LevelState::Draw()
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
			if (boss_) { boss_->Draw(); }

			data_->window.draw(scoreTxt_);
			data_->window.draw(playerLivesTxt_);

			data_->window.display();
		}
	}
	
	void LevelState::updateScore()
	{
		if (fading_ && lvlCompleteTime_ > 0)
		{
			playerScore_ += static_cast<int>(lvlCompletePoints_ / lvlCompleteTime_);
		}
		else if (player_->GetScore() != playerScore_)
		{
			playerScore_ = player_->GetScore();
			scoreTxt_.setString("SCORE: " + std::to_string(playerScore_));
		}
	}

	void LevelState::updateLives()
	{
		if (player_->GetLives() != playerLives_ && player_->GetLives() >= 0)
		{
			playerLives_ = player_->GetLives();
			playerLivesTxt_.setString("LIVES: " + std::to_string(playerLives_));
		}
	}


	void LevelState::backgroundAnimation()
	{
		if (lvlClock_.getElapsedTime().asSeconds() - backgroundAnimationTimeOffset_ > BACKGROUND_ANIMATION_FRAME_TIME)
		{
			if (backgroundCurrentTexture_ >= backgroundTextures_.size())
			{
				backgroundCurrentTexture_ = 0;
			}

			background_.setTexture(backgroundTextures_.at(backgroundCurrentTexture_++));
			backgroundAnimationTimeOffset_ = lvlClock_.getElapsedTime().asSeconds();
		}
	}	

	void LevelState::handlePowerUpsSpawning()
	{
		if (lvlClock_.getElapsedTime().asSeconds() - powerUpsSpawnTimeOffset_ > powerUpsSpawnTime_)
		{
			auto const powerUpType{ Random<int>(1, 3) };
			auto const startMoveDirection{ Random<int>(0, 1) == 0 ? DirectionX::left : DirectionX::right };

			switch (powerUpType)
			{
			case 1:
				powerUpShip_ = std::make_unique<LivesPowerUp>(data_, POWERUPS_MOVEMENT_SPEED,
					startMoveDirection, LIVES_POWERUP_VALUE,
					data_->assets.GetTexturesArray("Lives PowerUp"), ENEMY_ANIMATION_FRAME_TIME, lvlClock_);
				break;
			case 2:
				powerUpShip_ = std::make_unique<SpeedPowerUp>(data_, POWERUPS_MOVEMENT_SPEED,
					startMoveDirection, SPEED_POWERUP_VALUE,
					data_->assets.GetTexturesArray("Speed PowerUp"), ENEMY_ANIMATION_FRAME_TIME, lvlClock_);
				break;
			case 3:
				powerUpShip_ = std::make_unique<BulletsSpeedPowerUp>(data_, POWERUPS_MOVEMENT_SPEED,
					startMoveDirection, BULLETS_SPEED_POWERUP_VALUE,
					data_->assets.GetTexturesArray("Bullets Speed PowerUp"), ENEMY_ANIMATION_FRAME_TIME, lvlClock_);
				break;
			default:
				break;
			}
			powerUpsSpawnTimeOffset_ = lvlClock_.getElapsedTime().asSeconds();
		}
	}
}
