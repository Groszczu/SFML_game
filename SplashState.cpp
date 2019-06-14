#include "SplashState.hpp"
#include <utility>
#include "DEFINITIONS.hpp"
#include "Level1State.hpp"

namespace rstar
{
	SplashState::SplashState(GameDataPtr data) : data_(std::move(data))
	{
		data_->assets.LoadTexture("Level Background", { LEVEL_BACKGROUND1_FILEPATH, LEVEL_BACKGROUND2_FILEPATH, LEVEL_BACKGROUND3_FILEPATH });

		data_->assets.LoadTexture("Splash State Background", { SPLASH_STATE_BACKGROUND_FILEPATH });
		data_->assets.LoadTexture("Start Button", { START_BUTTON_FILEPATH });
		data_->assets.LoadTexture("Start Button Pressed", { START_BUTTON_PRESSED_FILEPATH });
		
		background_.setTexture(data_->assets.GetTexture("Splash State Background"));
		startButton_.setTexture(data_->assets.GetTexture("Start Button"));

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

			if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
			{
				data_->window.close();
			}
		}

		if (data_->input.IsSpriteClicked(startButton_, sf::Mouse::Button::Left, data_->window) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			fading_ = true;
		}

		if (data_->input.IsMouseOverSprite(startButton_, data_->window))
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
			data_->stateMachine.SetState(std::make_unique<Level1State>(data_), true);
		}

		handleButtonAnimation();
	}

	void SplashState::Draw()
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
			fading_ = false;
			initial_ = true;
		}
		else
		{
			data_->window.clear();
			data_->window.draw(background_);
			data_->window.draw(startButton_);
			data_->window.display();
		}
	}

	void SplashState::handleButtonAnimation()
	{
		if (mouseOverButton_)
		{
			startButton_.setTexture(data_->assets.GetTexture("Start Button Pressed"));
		}
		else if (clock_.getElapsedTime().asSeconds() > BUTTON_ANIMATION_FRAME_TIME && clock_.getElapsedTime().asSeconds() < 2 * BUTTON_ANIMATION_FRAME_TIME)
		{
			startButton_.setTexture(data_->assets.GetTexture("Start Button Pressed"));
		}
		else if (clock_.getElapsedTime().asSeconds() > 2 * BUTTON_ANIMATION_FRAME_TIME)
		{
			startButton_.setTexture(data_->assets.GetTexture("Start Button"));
			clock_.restart();
		}
	}
}
