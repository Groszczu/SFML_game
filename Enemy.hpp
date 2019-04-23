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
		bool IsTouchingGameObj(GameObject &other) const;


		void Update() override;
		void Draw() const override;

		friend void HandlePlayerBulletEnemiesIntersection(Enemies &e, PlayerShip &ship);
		friend void HandleEnemiesPlayerIntersection(Enemies &e, PlayerShip & ship);
		friend void HandleEnemiesShooting(Enemies &e, PlayerShip &ship);

	private:
		bool isDestroyed_{ false };
		bool toRemove_{ false };


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
		Enemies(GameDataPtr data, unsigned int enemiesCount, sf::Vector2f firstEnemyPos, sf::Clock &lvlClockRef);

		unsigned int GetEnemiesCount() const { return enemiesCount_; }
		void Shoot(sf::Vector2f const& startPosition);

		void Update();
		void Draw() const;

		static Direction EnemiesMoveDirection;
		static float EnemiesMovementSpeed;
		static bool MoveForward;
		static float EnemiesBulletsSpeed;

		friend void HandlePlayerBulletEnemiesIntersection(Enemies &e, PlayerShip &ship);
		friend void HandleEnemiesPlayerIntersection(Enemies &e, PlayerShip & ship);
		friend void HandleEnemiesShooting(Enemies &e, PlayerShip &ship);

	private:
		std::vector<std::unique_ptr<Enemy>> enemies_;
		std::vector<std::unique_ptr<Bullet>> bullets_;
		unsigned int enemiesCount_;

		sf::Clock &lvlClockRef_;
		float moveForwardTimeOffset_{ 0.f };
		float shotDelayTimeOffset_{ 0.f };

		GameDataPtr data_;

		void removeDestroyedEnemies();
		void handleShooting();
	};

	// friend free function to handle interactions between PlayerShip and Enemies
	void HandlePlayerBulletEnemiesIntersection(Enemies &e, PlayerShip &ship);

	void HandleEnemiesPlayerIntersection(Enemies &e, PlayerShip &ship);

	void HandleEnemiesShooting(Enemies &e, PlayerShip &ship);
}
