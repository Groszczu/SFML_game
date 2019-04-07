#pragma once
#include "Game.hpp"
#include "PlayerShip.hpp"
#include <thread>

namespace rstar
{
	class Level1 final : public State
	{
	public:
		explicit Level1(GameDataPtr data);
		~Level1();

		void HandleInput() override;
		void Update() override;
		void Draw() override;

	private:
		GameDataPtr data_;
		sf::Sprite background_;
		sf::Clock clock_;

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
	};
}

