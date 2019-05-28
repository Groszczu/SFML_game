#include "ScoreDisplayState.hpp"
#include "DEFINITIONS.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include "SplashState.hpp"

namespace rstar
{
	ScoreDisplayState::ScoreDisplayState(GameDataPtr data, int const playerScore, std::string fileName)
		: data_(std::move(data)), playerScore_(playerScore), fileName_(std::move(fileName))
	{
		data_->assets.LoadTexture("Score state Background", SCORE_STATE_BACKGROUND);

		background_.setTexture(data_->assets.GetTexture("Score state Background"));

		playerScoreTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		playerScoreTxt_.setFillColor(sf::Color::Yellow);
		playerScoreTxt_.setCharacterSize(MENU_FONT_SIZE);
		playerScoreTxt_.setPosition(WINDOW_WIDTH / 2.8f, WINDOW_HEIGHT * 0.75f);
		playerScoreTxt_.setString("SCORE: " + std::to_string(playerScore_));
			
		playerNickTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		playerNickTxt_.setFillColor(sf::Color::Yellow);
		playerNickTxt_.setCharacterSize(MENU_FONT_SIZE);
		playerNickTxt_.setPosition(WINDOW_WIDTH / 2.8f, WINDOW_HEIGHT * 0.75f - 2*MENU_FONT_SIZE);
		playerNickTxt_.setString("NAME: ");

		scoreTableTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		scoreTableTxt_.setFillColor(sf::Color::Yellow);
		scoreTableTxt_.setCharacterSize(SCORE_TABLE_FONT_SIZE);
		scoreTableTxt_.setPosition(WINDOW_WIDTH / 5.f, WINDOW_HEIGHT / 6.f);

		pressEnterTxt_.setFont(data_->assets.GetFont("Pixel Font"));
		pressEnterTxt_.setFillColor(sf::Color::Yellow);
		pressEnterTxt_.setCharacterSize(MENU_FONT_SIZE);
		pressEnterTxt_.setPosition(WINDOW_WIDTH / 2.72f, WINDOW_HEIGHT * 0.9f);
		pressEnterTxt_.setString("PRESS ENTER");
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
				}
				else if (scoreCalculated_ && ev.text.unicode == 13)
				{
					fading_ = true;
				}
				break;
			default:
				break;
			}
		}
	}

	void ScoreDisplayState::Update()
	{
		if(fading_)
		{
			data_->stateMachine.SetState(std::make_unique<SplashState>(data_));
		}

		if (!nameEntered_)
		{
			playerNickTxt_.setString("NAME: " + playerNick_);
		}
		else if (!scoreCalculated_)
		{
			loadScores();
			generateScoreTable();
			writeScores();
			
			scoreCalculated_ = true;
		}
		else
		{
			animateTxt();
		}
	}

	void ScoreDisplayState::loadScores()
	{
		std::ifstream inputScoreFile{ fileName_ };
		if (inputScoreFile)
		{
			std::string nick, score;
			std::string line;

			while (std::getline(inputScoreFile, line))
			{
				std::istringstream lineStream{ line };
				if (std::getline(lineStream, nick, ';') && std::getline(lineStream, score))
				{
					if (nick.length() > PLAYER_NICK_MAX_LENGTH)
					{
						nick = nick.substr(0, PLAYER_NICK_MAX_LENGTH);
					}

					score.erase(std::remove_if(begin(score), end(score), isspace), end(score));

					if (!nick.empty() && std::none_of(begin(nick), end(nick), isdigit) &&
						!score.empty() && std::all_of(begin(score), end(score), isdigit) && std::stoi(score) <= LVL1_MAX_POINTS)
					{
						scoreTable_.emplace_back(std::make_pair(nick, std::stoi(score)));
					}
				}
			}
			inputScoreFile.close();
		}
	}

	void ScoreDisplayState::writeScores() const
	{
		std::ofstream outputScoreFile{ fileName_, std::ios::trunc };
		if (outputScoreFile)
		{
			std::string nextLine{};
			for (auto const& playerData : scoreTable_)
			{
				outputScoreFile << nextLine << playerData.first << ';' << playerData.second;
				nextLine = '\n';
			}
			outputScoreFile.close();
		}
	}

	void ScoreDisplayState::generateScoreTable()
	{
		scoreTable_.emplace_back(std::make_pair(playerNick_, playerScore_));
		std::sort(begin(scoreTable_), end(scoreTable_), [](auto const& lhs, auto const& rhs) { return lhs.second > rhs.second; });
		if (scoreTable_.size() > SCORE_TABLE_MAX_SIZE)
		{
			scoreTable_.resize(SCORE_TABLE_MAX_SIZE);
		}
		
		std::ostringstream scores{};
		unsigned place{};

		scores << '\t' << std::left << std::setfill(' ') << std::setw(20) << "nick" << "score\n\n";
		for (auto const& playerData : scoreTable_)
		{
			scores << ++place << '\t' << std::left << std::setfill(' ') << std::setw(20) << playerData.first << playerData.second << "\n\n";
		}
		scoreTableTxt_.setString(scores.str());
	}

	void ScoreDisplayState::animateTxt()
	{
		if (animationClock_.getElapsedTime().asSeconds() > BUTTON_ANIMATION_DURATION)
		{
			if (pressEnterTxt_.getFillColor() == sf::Color::Yellow)
			{
				pressEnterTxt_.setFillColor(sf::Color::Red);
			}
			else
			{
				pressEnterTxt_.setFillColor(sf::Color::Yellow);
			}
			animationClock_.restart();
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
			if (scoreCalculated_)
			{
				data_->window.draw(scoreTableTxt_);
				data_->window.draw(pressEnterTxt_);
			}
			data_->window.display();
		}
	}
}
