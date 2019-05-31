#pragma once
#include "GameObject.hpp"
#include "Bullet.hpp"
#include "Game.hpp"
#include "PlayerShip.hpp"

namespace rstar
{
	enum Direction
	{
		left = -1,
		right = 1
	};

	class Enemy : public GameObject
	{
	public:
		Enemy(GameDataPtr data, sf::Vector2f startPosition, sf::Clock &clock);

		bool IsDestroyed() const { return isDestroyed_; }
		bool IsToRemove() const { return toRemove_; }
		bool IsCharging() const { return isCharging_; }
		void Charge() { isCharging_ = true; }


		void Update() override;
		void Draw() const override;

		friend class InteractionsHandler;

	private:
		bool isDestroyed_{ false };
		bool toRemove_{ false };
		bool isCharging_{ false };


		// variable to 'watch' enemies clock
		sf::Clock &clockRef_;

		float baseAnimationTimeOffset_{ 0.f };
		float destroyAnimationTimeOffset_{ 0.f };

		unsigned short currentTexture_{ 1 };
		unsigned short currentDestroyTexture_{ 0 };

		void animate();
		void animateDestroy();
		void handleMovement();
	};

	class Enemies : public BaseBullet
	{
	public:
		Enemies(GameDataPtr data, unsigned enemiesCount, float movementSpeed, float bulletsSpeed, float chargingSpeed,
			sf::Vector2f firstEnemyPos, float space, sf::Clock &lvlClockRef);

		unsigned int GetEnemiesCount() const { return enemiesCount_; }
		void Shoot(sf::Vector2f const& startPosition);

		void Update();
		void Draw() const;

		static Direction MoveDirection;
		static float MovementSpeed;
		static bool MoveForward;
		static float BulletsSpeed;
		static float ChargingSpeed;

		friend class InteractionsHandler;

	private:
		GameDataPtr data_;
		sf::Clock &lvlClockRef_;

		std::vector<std::unique_ptr<Enemy>> enemies_;
		std::vector<std::unique_ptr<Bullet>> bullets_;
		unsigned int enemiesCount_;

		float moveForwardTimeOffset_{ 0.f };
		float shotDelayTimeOffset_{ 0.f };

		void removeDestroyedEnemies();
		void handleCharging(unsigned int enemiesCharging);
		void reorderEnemies();
	};
}

