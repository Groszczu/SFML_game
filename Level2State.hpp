#pragma once

#include "Game.hpp"
#include "PlayerShip.hpp"
#include "Enemy.hpp"
#include "State.hpp"
#include <thread>

namespace rstar
{
	class Level2State final : public State
	{
	public:
		Level2State(GameDataPtr data, int playerLives, int playerScore);

		Level2State(Level2State const& other) = delete;
		Level2State(Level2State && other) = delete;
		Level2State& operator=(Level2State const& other) = delete;
		Level2State& operator=(Level2State&& other) = delete;
		~Level2State();

		void HandleInput() override;
		void Update() override;
		void Draw() override;

	private:
		GameDataPtr data_;
		sf::Sprite background_;

		int playerLives_;
		sf::Text playerLivesTxt_;

		int playerScore_;
		sf::Text scoreTxt_;

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

		void updateScore();
		void updateLives();
	};
}


