#pragma once
#include "GameObject.hpp"
#include "Bullet.hpp"
#include "DEFINITIONS.hpp"
#include <vector>
#include <memory>

namespace rstar
{
	class Enemies;
	class PlayerShip final : public GameObject, public BaseBullet
	{
	public:
		PlayerShip(GameDataPtr data, sf::Clock &clockRef);

		void Shoot();
		int GetScore() const { return score_; }
		int GetLives() const { return lives_; }

		void Update() override;
		void Draw() const override;

		friend class InteractionsHandler;
		
	private:
		float movementSpeed_{ PLAYER_START_SPEED };
		float bulletsSpeed_{ PLAYER_START_BULLET_SPEED };

		std::vector<std::unique_ptr<Bullet>> bullets_;

		int lives_{ 3 };
		int score_{ 0 };

		bool hit_{ false };

		sf::Clock &clockRef_;
		float shotDelayTimeOffset_{ 0 };
		float animationTimeOffset_{ 0 };
		int currentHitTexture_{ 0 };

		void handleMovement();
		void handleShooting();
		void animateHit();
	};
}
