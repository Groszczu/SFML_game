#pragma once
#include "Bullet.hpp"
#include "Game.hpp"
#include "PlayerShip.hpp"
#include "Animatable.hpp"

namespace rstar
{
	enum class DirectionX
	{
		left = -1,
		right = 1
	};

		class Enemy : public Animatable
	{
	public:
		Enemy(GameDataPtr data, sf::Vector2f startPosition,
			std::vector<sf::Texture> const& textures, float frameTime, sf::Clock const& clock);

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

		float destroyAnimationTimeOffset_{ 0.f };

		unsigned short currentDestroyTexture_{ 0 };

		void animateDestroy();
		void handleMovement();
	};

	class Enemies : public BaseBullet
	{
	public:
		Enemies(GameDataPtr data, unsigned enemiesCount, float movementSpeed, float bulletsSpeed, float chargingSpeed, unsigned enemiesCharging,
			sf::Vector2f firstEnemyPos, float space, sf::Clock &lvlClockRef);

		unsigned int GetEnemiesCount() const { return enemiesCount_; }
		void Shoot(sf::Vector2f const& startPosition);

		void Update();
		void Draw() const;

		static DirectionX MoveDirection;
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

		const unsigned enemiesCharging_;

		float moveForwardTimeOffset_{ 0.f };
		float shotDelayTimeOffset_{ 0.f };

		void removeDestroyedEnemies();
		void handleCharging(unsigned int enemiesCharging);
		void reorderEnemies();
	};
}

