#include "Level1State.hpp"
#include "DEFINITIONS.hpp"
#include "Enemy.hpp"
#include <thread>
#include "SplashState.hpp"
#include "ScoreDisplayState.hpp"

namespace rstar
{
	Level1State::Level1State(GameDataPtr data) : data_{ std::move(data) }
	{
		data_->assets.LoadTexture("Level Background1", LEVEL_BACKGROUND1_FILEPATH);
		data_->assets.LoadTexture("Level Background2", LEVEL_BACKGROUND2_FILEPATH);
		data_->assets.LoadTexture("Level Background3", LEVEL_BACKGROUND3_FILEPATH);

		data_->assets.LoadTexture("Player Ship", PLAYER_SHIP_FILEPATH);
		data_->assets.LoadTexture("Hit Player Ship", HIT_PLAYER_SHIP_FILEPATH);
		data_->assets.LoadTexture("Player Bullet", PLAYER_BULLET_FILEPATH);
		data_->assets.LoadTexture("Enemy Bullet", ENEMY_BULLET_FILEPATH);

		data_->assets.LoadTexture("Red Enemy1", RED_ENEMY1_FILEPATH);
		data_->assets.LoadTexture("Red Enemy2", RED_ENEMY2_FILEPATH);
		data_->assets.LoadTexture("Red Enemy3", RED_ENEMY3_FILEPATH);

		data_->assets.LoadTexture("Enemy dst1", ENEMY_DST1_FILEPATH);
		data_->assets.LoadTexture("Enemy dst2", ENEMY_DST2_FILEPATH);
		data_->assets.LoadTexture("Enemy dst3", ENEMY_DST3_FILEPATH);
		data_->assets.LoadTexture("Enemy dst4", ENEMY_DST4_FILEPATH);

		data_->assets.LoadFont("Pixel Font", PIXEL_FONT_FILEPATH);

		background_.setTexture(data_->assets.GetTexture("Level Background1"));

		scoreTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		scoreTxt_.setCharacterSize(IN_GAME_FONT_SIZE);
		scoreTxt_.setPosition(0, WINDOW_HEIGHT - IN_GAME_FONT_SIZE);
		scoreTxt_.setString("SCORE: " + std::to_string(playerScore_));

		playerLivesTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		playerLivesTxt_.setCharacterSize(IN_GAME_FONT_SIZE);
		playerLivesTxt_.setPosition(WINDOW_WIDTH - 10 * IN_GAME_FONT_SIZE, WINDOW_HEIGHT - IN_GAME_FONT_SIZE);
		playerLivesTxt_.setString("LIVES: " + std::to_string(playerLives_));

		player_ = std::make_unique<PlayerShip>(data_, lvlClock_);
		enemies_ = std::make_unique<Enemies>(data_, LVL1_ENEMIES_COUNT, sf::Vector2f{ ENEMIES_SIDE_MARGIN, ENEMIES_TOP_MARGIN }, lvlClock_);

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
			// TODO: display player score (maybe on other state)
			updateScore();
			data_->stateMachine.SetState(std::make_unique<ScoreDisplayState>(data_, playerScore_, SCORES_FILEPATH), true);
		}

		InteractionsHandler::Run(*enemies_, *player_);

		player_->Update();
		enemies_->Update();

		updateScore();
		updateLives();

		if (player_->GetLives() <= 0)
		{
			// no points for losing lvl
			lvlCompleteTime_ = -1.f;
			fading_ = true;
		}

		if (enemies_->GetEnemiesCount() <= 0)
		{
			// TODO: display player score (maybe on other state)
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
			if (lvlClock_.getElapsedTime().asSeconds() - localTimeOffset > BACKGROUND_ANIMATION_DURATION)
			{
				switch (backgroundPointer_)
				{
				case 1:
					background_.setTexture(data_->assets.GetTexture("Level Background2"));
					backgroundPointer_++;
					break;
				case 2:
					background_.setTexture(data_->assets.GetTexture("Level Background3"));
					backgroundPointer_++;
					break;
				case 3:
					background_.setTexture(data_->assets.GetTexture("Level Background1"));
					backgroundPointer_ = 1;
					break;
				default:
					backgroundPointer_ = 1;
					break;
				}
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
}
