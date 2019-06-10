#pragma once
#include "State.hpp"
#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include "PlayerShip.hpp"
#include "PowerUpShip.hpp"
#include "Boss.hpp"

namespace rstar
{
	class LevelState : public State
	{
	protected:
		LevelState(GameDataPtr data, std::string const& backgroundTextureName, float powerUpsSpawnTime,
			unsigned lvlCompletionPoints);

		void HandleInput() override;
		void Update() override;
		void Draw() override;

	protected:
		GameDataPtr data_;
		sf::Sprite background_;
		std::vector<sf::Texture> const& backgroundTextures_;

		int playerScore_{ 0 };
		sf::Text scoreTxt_;

		unsigned playerLives_{ PLAYER_LIVES };
		sf::Text playerLivesTxt_;

		sf::Clock lvlClock_;

		float powerUpsSpawnTime_;
		float powerUpsSpawnTimeOffset_{ 0.f };

		float lvlCompleteTime_{ 0.f };
		unsigned lvlCompletePoints_;

		// it's value is equal to index number of background image displayed currently (animating background) 
		unsigned backgroundCurrentTexture_{ 0 };
		float backgroundAnimationTimeOffset_{ 0.f };
		void backgroundAnimation();

		std::unique_ptr<PlayerShip> player_;
		std::unique_ptr<Enemies> enemies_;
		std::unique_ptr<PowerUpShip> powerUpShip_;
		std::unique_ptr<Boss> boss_;

		virtual void updateScore();
		virtual void updateLives();
		virtual void handlePowerUpsSpawning();
	};
}
