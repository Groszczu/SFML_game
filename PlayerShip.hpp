#pragma once
#include <vector>
#include <memory>
#include "GameObject.hpp"
#include "Bullet.hpp"
#include "DEFINITIONS.hpp"

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

		friend void HandleBulletsIntersection(Enemies &e, PlayerShip &ship);
		friend void HandleIntersection(Enemies &e, PlayerShip &ship);
		
	private:
		float movementSpeed_{ PLAYER_START_SPEED };
		float bulletsSpeed_{ PLAYER_START_BULLET_SPEED };

		std::vector<std::unique_ptr<Bullet>> bullets_;

		int lives_{ 3 };
		int score_{ 0 };

		sf::Clock &shotClockRef_;
		float shotDelayTimeOffset_{ 0 };

		void handleMovement();
		void handleShooting();
	};
}
