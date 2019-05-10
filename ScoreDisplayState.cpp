#include "ScoreDisplayState.hpp"
#include "DEFINITIONS.hpp"
#include <fstream>
#include <iostream>

namespace rstar
{
	ScoreDisplayState::ScoreDisplayState(GameDataPtr data, int const playerScore, std::string const& fileName)
		: data_(std::move(data)), playerScore_(playerScore), scoreFile_(fileName, std::ios::app)
	{
		data_->assets.LoadTexture("Score state Background", SCORE_STATE_BACKGROUND);

		background_.setTexture(data_->assets.GetTexture("Score state Background"));
	}

	ScoreDisplayState::~ScoreDisplayState()
	{
		scoreFile_.close();
	}


	void ScoreDisplayState::HandleInput()
	{
		sf::Event ev{};

		while (data_->window.pollEvent(ev))
		{
			switch (ev.type)
			{
			case sf::Event::Closed:
				data_->window.close();
				break;
			case sf::Event::TextEntered:
				if (ev.text.unicode < 128)
				{
					playerNick_ += static_cast<char>(ev.text.unicode);
					std::cout << playerNick_ << ": " << playerScore_ << std::endl;
				}
				break;
			default:
				break;
			}
		}
	}

	void ScoreDisplayState::Update()
	{
		//TODO: saving high score to file
	}

	void ScoreDisplayState::Draw()
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
			data_->window.display();
		}
	}
}
