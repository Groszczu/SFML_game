#include "SplashState.hpp"
#include <utility>
#include "DEFINITIONS.hpp"
#include <iostream>
#include "Level1.hpp"

namespace rstar
{
	SplashState::SplashState(GameDataPtr data) : data_(std::move(data))
	{
		data_->assets.LoadTexture("Splash State Background", SPLASH_STATE_BACKGROUND_FILEPATH);
		data_->assets.LoadTexture("Start Button", START_BUTTON_FILEPATH);
		data_->assets.LoadTexture("Start Button Pressed", START_BUTTON_PRESSED_FILEPATH);
		
		// setting sprites
		background_.setTexture(data_->assets.GetTexture("Splash State Background"));
		startButton_.setTexture(data_->assets.GetTexture("Start Button"));

		// setting start button size and position
		startButton_.setScale({ 2.f, 2.f });
		startButton_.setPosition(WINDOW_WIDTH / 2.f - startButton_.getGlobalBounds().width / 2.f, WINDOW_HEIGHT / 2.15f);
	}

	void SplashState::HandleInput()
	{
		sf::Event ev{};

		while (data_->window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				data_->window.close();
			}
		}

		if (data_->input.IsSpriteClicked(startButton_, sf::Mouse::Button::Left, data_->window))
		{
			fading_ = true;
		}
		if (data_->input.IsMouseOverSprite(startButton_, sf::Mouse::Button::Left, data_->window))
		{
			mouseOverButton_ = true;
		}
		else
		{
			mouseOverButton_ = false;
		}
	}

	void SplashState::Update()
	{
		if (fading_)
		{			
			data_->stateMachine.SetState(std::make_unique<Level1>(data_), true);
			std::cout << "Go to menu\n";
		}
		if (mouseOverButton_)
		{
			startButton_.setTexture(data_->assets.GetTexture("Start Button Pressed"));
		}
		else if (clock_.getElapsedTime().asSeconds() > BUTTON_ANIMATION_DURATION && clock_.getElapsedTime().asSeconds() < 2 * BUTTON_ANIMATION_DURATION)
		{
			startButton_.setTexture(data_->assets.GetTexture("Start Button Pressed"));
		}
		else if (clock_.getElapsedTime().asSeconds() > 2 * BUTTON_ANIMATION_DURATION)
		{
			startButton_.setTexture(data_->assets.GetTexture("Start Button"));
			clock_.restart();
		}
	}

	void SplashState::Draw()
	{
		if (fading_)
		{
			Fading(FADE_FRAME_TIME, data_->window);
		}
		else
		{
			data_->window.clear();
			data_->window.draw(background_);
			data_->window.draw(startButton_);
			data_->window.display();
		}
	}
}
