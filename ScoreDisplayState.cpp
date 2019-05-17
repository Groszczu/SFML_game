#include "ScoreDisplayState.hpp"
#include "DEFINITIONS.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace rstar
{
	ScoreDisplayState::ScoreDisplayState(GameDataPtr data, int const playerScore, std::string const& fileName)
		: data_(std::move(data)), playerScore_(playerScore), scoreFile_(fileName)
	{
		data_->assets.LoadTexture("Score state Background", SCORE_STATE_BACKGROUND);

		background_.setTexture(data_->assets.GetTexture("Score state Background"));

		playerScoreTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		playerScoreTxt_.setFillColor(sf::Color::Yellow);
		playerScoreTxt_.setCharacterSize(MENU_FONT_SIZE);
		playerScoreTxt_.setPosition(WINDOW_WIDTH / 2.8f, WINDOW_HEIGHT * 3.f / 4.f);
		playerScoreTxt_.setString("SCORE: " + std::to_string(playerScore_));
			
		playerNickTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		playerNickTxt_.setFillColor(sf::Color::Yellow);
		playerNickTxt_.setCharacterSize(MENU_FONT_SIZE);
		playerNickTxt_.setPosition(WINDOW_WIDTH / 2.8f, WINDOW_HEIGHT * 3.f / 4.f - 2*MENU_FONT_SIZE);

		playerNickTxt_.setString("NAME: ");
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
				if (!nameEntered_)
				{
					if (ev.text.unicode == 13 && !playerNick_.empty()) // ENTER Key pressed
					{
						nameEntered_ = true;
					}
					else if (ev.text.unicode == 8) // BS Key pressed
					{
						if (!playerNick_.empty())
						{
							playerNick_.pop_back();
						}
					}
					else if (ev.text.unicode >= 65 && ev.text.unicode <= 122)
					{
						if (playerNick_.length() < PLAYER_NICK_MAX_LENGTH)
						{
							playerNick_ += static_cast<char>(ev.text.unicode);
						}
					}
					std::cout << playerNick_ << std::endl;
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
		if (!nameEntered_)
		{
			playerNickTxt_.setString("NAME: " + playerNick_);
		}
		else
		{
			loadScores();
			scoreTable_.emplace_back(std::make_pair(playerNick_, playerScore_));
			std::sort(begin(scoreTable_), end(scoreTable_), [](auto const& lhs, auto const& rhs) { return lhs.second > rhs.second; });
		}
	}

	void ScoreDisplayState::loadScores()
	{
		std::string nick, score;
		std::string line;
		while (std::getline(scoreFile_, line))
		{
			std::istringstream lineStream{ line };
			if (std::getline(lineStream, nick, ';') && std::getline(lineStream, score))
			{
				if (nick.length() > PLAYER_NICK_MAX_LENGTH)
				{
					nick = nick.substr(0, PLAYER_NICK_MAX_LENGTH);
				}

				score.erase(std::remove_if(begin(score), end(score), isspace), end(score));
				
				if (!nick.empty() &&
					!score.empty() &&
					std::all_of(begin(score), end(score), isdigit))
				{
					scoreTable_.emplace_back(std::make_pair(nick, std::stoi(score)));
				}
			}
		}
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
			if (!nameEntered_)
			{
				data_->window.draw(playerNickTxt_);
				data_->window.draw(playerScoreTxt_);
			}
			data_->window.display();
		}
	}
}
