#include "DEFINITIONS.hpp"
#include "GameObject.hpp"
#include "Enemy.hpp"
#include <algorithm>

namespace rstar
{
	Enemy::Enemy(GameDataPtr data, sf::Vector2f startPosition, sf::Clock &clock, sf::Clock &dstClock)
		: GameObject(data), clockRef_(clock), dstClockRef_(dstClock)
	{
		sprite_.setTexture(data_->assets.GetTexture("Red Enemy1"));
		sprite_.setScale(2.f, 2.f);
		sprite_.setRotation(180);
		sprite_.setPosition(startPosition);
	}

	void Enemy::animate_()
	{
		if (clockRef_.getElapsedTime().asSeconds() > ENEMY_ANIMATION_DURATION)
		{
			switch (currentTexture_)
			{
			case 1: sprite_.setTexture(data_->assets.GetTexture("Red Enemy2"));
				++currentTexture_;
				break;
			case 2: sprite_.setTexture(data_->assets.GetTexture("Red Enemy3"));
				++currentTexture_;
				break;
			case 3: sprite_.setTexture(data_->assets.GetTexture("Red Enemy2"));
				++currentTexture_;
				break;
			case 4: sprite_.setTexture(data_->assets.GetTexture("Red Enemy1"));
				currentTexture_ = 1;
				break;
			default:
				break;
			}
		}
	}

	void Enemy::animateDestroy_()
	{
		if (dstClockRef_.getElapsedTime().asSeconds() > BOOM_ANIMATION_DURATION)
		{
			switch (currentDestroyTexture_)
			{
			case 0: sprite_.setTexture(data_->assets.GetTexture("Enemy dst1"));
				++currentDestroyTexture_;
				break;
			case 1: sprite_.setTexture(data_->assets.GetTexture("Enemy dst2"));
				++currentDestroyTexture_;
				break;
			case 2: sprite_.setTexture(data_->assets.GetTexture("Enemy dst3"));
				++currentDestroyTexture_;
				break;
			case 3: sprite_.setTexture(data_->assets.GetTexture("Enemy dst4"));
				++currentDestroyTexture_;
				break;
			case 4:
				toRemove_ = true;
				break;
			default:
				break;
			}
		}
	}


	void Enemy::Update()
	{
		if (isDestroyed_)
		{
			animateDestroy_();
		}
		else
		{
			animate_();
		}
	}

	void Enemy::Draw() const
	{
		data_->window.draw(sprite_);
	}

	Enemies::Enemies(GameDataPtr data, unsigned int enemiesCount, sf::Vector2f firstEnemyPos)
		: enemiesCount_(enemiesCount), data_(std::move(data))
	{
		std::generate_n(std::back_inserter(enemies_), enemiesCount,
			[&]
		{
			firstEnemyPos.x += ENEMIES_WIDTH;
			if (firstEnemyPos.x + ENEMIES_WIDTH >= WINDOW_WIDTH)
			{
				firstEnemyPos.x = 2*ENEMIES_WIDTH;
				firstEnemyPos.y += ENEMIES_HEIGHT;
			}
			return std::make_unique<Enemy>(data_, firstEnemyPos, enemiesClock_, destroyClock_);
		}
		);
		enemiesClock_.restart();
	}

	void Enemies::RemoveEnemy(std::vector<std::unique_ptr<Enemy>>::iterator & enemyIt)
	{
		enemies_.erase(enemyIt);
		--enemiesCount_;
	}

	void Enemies::Update()
	{
		for (auto &enemy : enemies_)
		{
			enemy->Update();
		}

		enemies_.erase(std::remove_if(begin(enemies_), end(enemies_), [](std::unique_ptr<Enemy> &enemy) { return enemy->IsToRemove(); }), end(enemies_));
		
		if (enemiesClock_.getElapsedTime().asSeconds() > ENEMY_ANIMATION_DURATION)
		{
			enemiesClock_.restart();
		}
		if (destroyClock_.getElapsedTime().asSeconds() > BOOM_ANIMATION_DURATION)
		{
			destroyClock_.restart();
		}
	}

	void Enemies::Draw() const
	{
		for (auto const& enemy : enemies_)
		{
			enemy->Draw();
		}
	}

	void RemovePlayerBullet(Enemies &e, PlayerShip &ship)
	{
		for (auto &enemy : e.enemies_)
		{
			ship.bullets_.erase(std::remove_if(begin(ship.bullets_), end(ship.bullets_),
				[&](auto &bullet)
			{
				auto intersect = bullet->GetBounds().intersects(enemy->GetBounds());
				if (intersect)
				{
					enemy->isDestroyed_ = true;
				}

				return intersect;
			}
			), end(ship.bullets_));
		}
	}
}
