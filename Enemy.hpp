#pragma once
#include "GameObject.hpp"
#include "Bullet.hpp"
#include "Game.hpp"
#include "PlayerShip.hpp"

namespace rstar {

	class Enemy : public GameObject, public BaseBullet
	{
	public:
		Enemy(GameDataPtr data, sf::Vector2f startPosition, sf::Clock &clock, sf::Clock &dstClock);

		bool IsToRemove() const { return toRemove_; }
		void Update() override;
		void Draw() const override;

		friend void RemovePlayerBullet(Enemies &e, PlayerShip &ship);

	private:
		bool isDestroyed_{ false };
		bool toRemove_{ false };
		sf::Clock &clockRef_;
		sf::Clock &dstClockRef_;
		unsigned short currentTexture_{ 1 };
		unsigned short currentDestroyTexture_{ 0 };

		void animate_();
		void animateDestroy_();
	};

	class Enemies
	{
	public:
		Enemies(GameDataPtr data, unsigned int enemiesCount, sf::Vector2f firstEnemyPos);

		void RemoveEnemy(std::vector<std::unique_ptr<Enemy>>::iterator & enemyIt);

		void Update();
		void Draw() const;

		friend void RemovePlayerBullet(Enemies &e, PlayerShip &ship);

	private:
		std::vector<std::unique_ptr<Enemy>> enemies_;
		unsigned int enemiesCount_;
		sf::Clock enemiesClock_;
		sf::Clock destroyClock_;
		GameDataPtr data_;
	};

	void RemovePlayerBullet(Enemies &e, PlayerShip &ship);
}
