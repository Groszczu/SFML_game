#pragma once
#include "Game.hpp"
#include "PlayerShip.hpp"
#include "Enemy.hpp"
#include "State.hpp"
#include "InteractionsHandler.hpp"
#include <thread>

namespace rstar
{
	class Level1State final : public State
	{
	public:
		explicit Level1State(GameDataPtr data);

		Level1State(Level1State const& other) = delete;
		Level1State(Level1State&& l1) = delete;
		Level1State& operator=(Level1State const& other) = delete;
		Level1State& operator=(Level1State&& other) = delete;
		~Level1State();

		void HandleInput() override;
		void Update() override;
		void Draw() override;

	private:
		GameDataPtr data_;
		sf::Sprite background_;

		int playerScore_{ 0 };
		sf::Text scoreTxt_;

		int playerLives_{ LVL1_PLAYER_LIVES };
		sf::Text playerLivesTxt_;

		sf::Clock lvlClock_;
		float lvlCompleteTime_{ 0.f };

		// bool variables to manage changing screens
		bool initial_{ true };
		bool fading_{ false };

		// it's value is equal to index number of background image displayed currently (animating background) 
		int backgroundPointer_{ 1 };

		// animating background thread
		std::thread backgroundThread_;
		void backgroundAnimation();
		bool stopThread_{ false };

		// player ship pointer
		std::unique_ptr<PlayerShip> player_;

		// enemies pointer
		std::unique_ptr<Enemies> enemies_;

		InteractionsHandler handler_;

		void updateScore();
		void updateLives();
	};
}

