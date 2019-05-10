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
		~ScoreDisplayState();

		void HandleInput() override;
		void Update() override;
		void Draw() override;
	private:
		GameDataPtr data_;
		const int playerScore_;
		std::fstream scoreFile_;
		
		std::string playerNick_{};

		sf::Sprite background_;

		bool initial_{ true };
		bool fading_{ false };
	};
}
