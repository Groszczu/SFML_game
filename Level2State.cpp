#include "Level2State.hpp"
#include "ScoreDisplayState.hpp"
#include "InteractionsHandler.hpp"

namespace rstar
{
	Level2State::Level2State(GameDataPtr data, int playerLives, int playerScore)
		: data_(std::move(data)), playerLives_(playerLives), playerScore_(playerScore), backgroundThread_(std::thread(&Level2State::backgroundAnimation, this)),
		player_(std::make_unique<PlayerShip>(data_, lvlClock_, playerLives_, playerScore, PLAYER_START_SPEED, PLAYER_START_BULLET_SPEED)) 
	{
		background_.setTexture(data_->assets.GetTexture("Level Background1"));

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
}
