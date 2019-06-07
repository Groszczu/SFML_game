#pragma once
#include "State.hpp"
#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include "PlayerShip.hpp"
#include "PowerUpShip.hpp"

namespace rstar
{
	class LevelState : public State
	{
	public:
		LevelState(GameDataPtr data, std::string const& backgroundTextureName, 
			unsigned enemiesCount, float enemiesMovementSpeed, float enemiesBulletsSpeed,
			float enemiesChargingSpeed, unsigned enemiesChargingAtOnce, unsigned enemiesLives,
			sf::Vector2f firstEnemySpawnPosition, float spaceBetweenEnemies, float powerUpsSpawnTime,
			unsigned lvlCompletionPoints, bool bossLvl = false);

		void HandleInput() override = 0;
		void Update() override = 0;
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

		// bool variables to manage changing screens
		bool initial_{ true };
		bool fading_{ false };

		// it's value is equal to index number of background image displayed currently (animating background) 
		unsigned backgroundCurrentTexture_{ 0 };
		float backgroundAnimationTimeOffset_{ 0.f };
		void backgroundAnimation();

		std::unique_ptr<PlayerShip> player_;
		std::unique_ptr<Enemies> enemies_;
		std::unique_ptr<PowerUpShip> powerUpShip_;

		virtual void updateScore();
		virtual void updateLives();
		virtual void handlePowerUpsSpawning();
	};
}
