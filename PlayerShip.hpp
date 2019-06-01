#pragma once
#include "GameObject.hpp"
#include "Bullet.hpp"
#include "DEFINITIONS.hpp"
#include <memory>

namespace rstar
{
	class Enemies;
	class PlayerShip final : public GameObject, public BaseBullet
	{
	public:
		PlayerShip(GameDataPtr data, sf::Clock &clockRef);
		PlayerShip(GameDataPtr data, sf::Clock &clockRef, int lives, int score, float movementSpeed, float bulletsSpeed);
		
		void Shoot();
		int GetScore() const { return score_; }
		int GetLives() const { return lives_; }
		void IncreaseLives(unsigned amount) { lives_ += amount; }

		void Update() override;
		void Draw() const override;

		friend class InteractionsHandler;
		
	private:
		sf::Clock &clockRef_;

		int lives_{ PLAYER_LIVES };
		int score_{ 0 };

		float movementSpeed_{ PLAYER_START_SPEED };
		float bulletsSpeed_{ PLAYER_START_BULLET_SPEED };

		std::unique_ptr<Bullet> bullet_;

		bool hit_{ false };

		float animationTimeOffset_{ 0 };
		int currentHitTexture_{ 0 };

		void handleMovement();
		void handleShooting();
		void animateHit();
	};
}
