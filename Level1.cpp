#include "Level1.hpp"
#include "DEFINITIONS.hpp"
#include <thread>

namespace rstar
{
	Level1::Level1(GameDataPtr data) : data_{ std::move(data) } 
	{	
		data_->assets.LoadTexture("Level Background1",LEVEL_BACKGROUND1_FILEPATH);
		data_->assets.LoadTexture("Level Background2",LEVEL_BACKGROUND2_FILEPATH);
		data_->assets.LoadTexture("Level Background3",LEVEL_BACKGROUND3_FILEPATH);
		data_->assets.LoadTexture("Player Ship", PLAYER_SHIP_FILEPATH);
		data_->assets.LoadTexture("Player Bullet", PLAYER_BULLET_FILEPATH);

		background_.setTexture(data_->assets.GetTexture("Level Background1"));
		
		player_ = std::make_unique<PlayerShip>(data_);
		backgroundThread_ = std::thread(&Level1::backgroundAnimation, this);
	}

	Level1::~Level1()
	{
		stopThread_ = true;
		if (backgroundThread_.joinable())
		{
			backgroundThread_.join();
		}
	}

	void Level1::HandleInput()
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

	void Level1::Update()
	{
		player_->Update();
	}

	void Level1::Draw()
	{
		if (initial_)
		{
			data_->window.draw(background_);
			FadingAway(SHADOW_FRAME_TIME, data_->window, background_);
			initial_ = false;
		}
		if (fading_)
		{
			Fading(SHADOW_FRAME_TIME, data_->window);
			fading_ = false;
		}
		else
		{
			data_->window.clear();
			data_->window.draw(background_);
			player_->Draw();
			data_->window.display();
		}		
	}

	void Level1::backgroundAnimation()
	{
		sf::Clock b_clock;
		while (!stopThread_)
		{
			if (b_clock.getElapsedTime().asSeconds() > BACKGROUND_ANIMATION_DURATION)
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
				b_clock.restart();
			}
		}
	}
}
