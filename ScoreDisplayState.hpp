#pragma once
#include <fstream>
#include "State.hpp"
#include "Game.hpp"

namespace rstar
{
	class ScoreDisplayState : public State
	{
	public:
		ScoreDisplayState(GameDataPtr data, int playerScore, std::string const& fileName);

		void HandleInput() override;
		void Update() override;
		void Draw() override;

	private:
		GameDataPtr data_;
		const int playerScore_;
		std::fstream scoreFile_;
		
		std::string playerNick_{};
		sf::Text playerNickTxt_;

		sf::Text playerScoreTxt_;

		sf::Sprite background_;

		std::vector<std::pair<std::string, int>> scoreTable_;

		bool initial_{ true };
		bool fading_{ false };
		bool nameEntered_{ false };

		void loadScores();
	};
}
