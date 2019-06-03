#pragma once
#include "Game.hpp"
#include "PlayerShip.hpp"
#include "Enemy.hpp"
#include "State.hpp"
#include <thread>
#include "PowerUpShip.hpp"

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

		int playerLives_{ PLAYER_LIVES };
		sf::Text playerLivesTxt_;

		sf::Clock lvlClock_;
		float lvlCompleteTime_{ 0.f };

		float powerUpsSpawnTimeOffset{ 0.f };

		// bool variables to manage changing screens
		bool initial_{ true };
		bool fading_{ false };

		// it's value is equal to index number of background image displayed currently (animating background) 
		unsigned backgroundCurrentTexture_{ 0 };

		// animating background thread
		std::thread backgroundThread_;
		void backgroundAnimation();
		bool stopThread_{ false };

		std::unique_ptr<PlayerShip> player_;
		std::unique_ptr<Enemies> enemies_;
		std::unique_ptr<PowerUpShip> powerUpShip_;

		void updateScore();
		void updateLives();
		void handlePowerUpsSpawning();
	};
}

