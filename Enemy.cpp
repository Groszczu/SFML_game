#include "DEFINITIONS.hpp"
#include "GameObject.hpp"
#include "Enemy.hpp"
#include <algorithm>
#include <iostream>

namespace rstar
{
	Direction Enemies::EnemiesMoveDirection = right;
	float Enemies::EnemiesMovementSpeed = ENEMIES_START_MOVEMENT_SPEED;
	bool Enemies::MoveForward = false;
	float Enemies::EnemiesBulletsSpeed = LVL1_ENEMIES_BULLETS_SPEED;

	// Start enemy---------------------------------------------------------------------------
	Enemy::Enemy(GameDataPtr data, sf::Vector2f startPosition, sf::Clock &clock)
		: GameObject(data), clockRef_(clock)
	{
		sprite_.setTexture(data_->assets.GetTexture("Red Enemy1"));
		sprite_.setScale(2.f, 2.f);
		sprite_.setRotation(180);
		sprite_.setPosition(startPosition);
	}

	void Enemy::animate()
	{
		if (clockRef_.getElapsedTime().asSeconds() - baseAnimationTimeOffset_ > ENEMY_ANIMATION_DURATION)
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
			baseAnimationTimeOffset_ = clockRef_.getElapsedTime().asSeconds();
		}
	}

	void Enemy::animateDestroy()
	{
		if (clockRef_.getElapsedTime().asSeconds() - destroyAnimationTimeOffset_ > BOOM_ANIMATION_DURATION)
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
			destroyAnimationTimeOffset_ = clockRef_.getElapsedTime().asSeconds();
		}
	}

	void Enemy::handleMovement()
	{
		if (IsOutOfScreen())
		{
			toRemove_ = true;
			return;
		}

		if (GetPosition().x > WINDOW_WIDTH)
		{
			Enemies::EnemiesMoveDirection = left;
		}
		if (GetPosition().x - ENEMIES_WIDTH < 0)
		{
			Enemies::EnemiesMoveDirection = right;
		}

		sf::Vector2f const moveDirection{Enemies::EnemiesMoveDirection * Enemies::EnemiesMovementSpeed, Enemies::MoveForward * Enemies::EnemiesMovementSpeed };

		sprite_.move(moveDirection);
	}

	bool Enemy::IsTouchingGameObj(GameObject& other) const
	{
		return !isDestroyed_ && GetBounds().intersects(other.GetBounds());
	}

	void Enemy::Update()
	{
		if (isDestroyed_)
		{
			animateDestroy();
		}
		else
		{
			animate();
			handleMovement();
		}
	}

	void Enemy::Draw() const
	{
		data_->window.draw(sprite_);
		
	}
	// End enemy---------------------------------------------------------------------------

	// Start enemies-----------------------------------------------------------------------
	Enemies::Enemies(GameDataPtr data, unsigned int enemiesCount, sf::Vector2f firstEnemyPos, sf::Clock &lvlClockRef)
		: enemiesCount_(enemiesCount), lvlClockRef_(lvlClockRef), data_(std::move(data))
	{
		std::generate_n(std::back_inserter(enemies_), enemiesCount,
			[&]
		{
			if (firstEnemyPos.x + ENEMIES_WIDTH >= WINDOW_WIDTH - ENEMIES_SIDE_MARGIN)
			{
				firstEnemyPos.x = ENEMIES_SIDE_MARGIN;
				firstEnemyPos.y += ENEMIES_HEIGHT + SPACE_BETWEEN_ENEMIES;
			}

			firstEnemyPos.x += ENEMIES_WIDTH + SPACE_BETWEEN_ENEMIES;

			return std::make_unique<Enemy>(data_, firstEnemyPos, lvlClockRef_);
		}
		);

		EnemiesMoveDirection = right;
		EnemiesMovementSpeed = ENEMIES_START_MOVEMENT_SPEED;
		MoveForward = false;
	}

	void Enemies::removeDestroyedEnemies()
	{
		enemies_.erase(std::remove_if(begin(enemies_), end(enemies_),
			[&](std::unique_ptr<Enemy> &enemy)
		{
			if (enemy->IsToRemove())
			{
				--enemiesCount_;
			}

			return enemy->IsToRemove();
		}), end(enemies_));
	}

	void Enemies::Update()
	{
		if (lvlClockRef_.getElapsedTime().asSeconds() - moveForwardTimeOffset_ > LVL1_ENEMIES_MOVE_FORWARD_TIME && !MoveForward)
		{
			MoveForward = true;
			moveForwardTimeOffset_ = lvlClockRef_.getElapsedTime().asSeconds();
		}
		if (MoveForward && lvlClockRef_.getElapsedTime().asSeconds() - moveForwardTimeOffset_ > LVL1_ENEMIES_MOVE_FORWARD_DURATION)
		{
			MoveForward = false;
			moveForwardTimeOffset_ = lvlClockRef_.getElapsedTime().asSeconds();
		}
		
		for (auto &enemy : enemies_)
		{
			enemy->Update();
		}

		for (auto &bullet : bullets_)
		{
			bullet->Update();
		}

		removeDestroyedEnemies();
	}

	void Enemies::Draw() const
	{		
		std::for_each(begin(enemies_), end(enemies_), 
			[](auto const& enemy)
		{
			if(!enemy->IsDestroyed())
			{
				enemy->Draw();
			}
		}
		);

		std::for_each(begin(bullets_), end(bullets_),
			[](auto const& bullet)
		{
			bullet->Draw();
		}
		);

		// draw destroyed enemies later to appear on top of other enemies
		std::for_each(begin(enemies_), end(enemies_), 
			[](auto const& enemy)
		{
			if(enemy->IsDestroyed())
			{
				enemy->Draw();
			}
		}
		);
	}
	
	void Enemies::Shoot(sf::Vector2f const& startPosition)
	{
		// putting new Bullet object in the bullets_ vector [as new Bullet unique_ptr]
		bullets_.emplace_back(std::make_unique<Bullet>(data_, startPosition, EnemiesBulletsSpeed));
	}
	// End enemies---------------------------------------------------------------------

	// Free functions------------------------------------------------------------------
	void HandlePlayerBulletEnemiesIntersection(Enemies &e, PlayerShip &ship)
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
					ship.score_ += LVL1_POINTS_FOR_ENEMY;
				}

				return intersect;
			}
			), end(ship.bullets_));

			e.bullets_.erase(std::remove_if(begin(e.bullets_), end(e.bullets_),
				[&](auto &bullet)
			{
				auto intersect = bullet->GetBounds().intersects(ship.GetBounds());
				if (intersect)
				{
					--ship.lives_;
				}

				return intersect;
			}
			), end(e.bullets_));
		}
	}
	void HandleEnemiesPlayerIntersection(Enemies &e, PlayerShip &ship)
	{
		std::for_each(begin(e.enemies_), end(e.enemies_),
		[&](auto &enemy)
		{
			if (!enemy->IsDestroyed() && enemy->GetBounds().intersects(ship.GetBounds()))
			{
				enemy->isDestroyed_ = true;
				--ship.lives_;
			}
		}
		);
	}

	void HandleEnemiesShooting(Enemies &e, PlayerShip &ship)
	{
		std::for_each(begin(e.enemies_), end(e.enemies_),
			[&e, &ship](auto &enemy)
		{
			if (e.lvlClockRef_.getElapsedTime().asSeconds() > LVL1_ENEMIES_START_SHOOT_DELAY
				&& e.lvlClockRef_.getElapsedTime().asSeconds() - e.shotDelayTimeOffset_ > ENEMIES_SHOT_DELAY
				&& enemy->GetPosition().x + ENEMIES_WIDTH > ship.GetPosition().x 
				&& enemy->GetPosition().x - ENEMIES_WIDTH < ship.GetPosition().x)
			{
				if (Random<float>(0, 100) < LVL1_ENEMIES_CHANCE_TO_SHOOT)
				{
					e.Shoot(sf::Vector2f{enemy->GetPosition().x - enemy->GetBounds().width/2.f, enemy->GetPosition().y});
					e.shotDelayTimeOffset_ = e.lvlClockRef_.getElapsedTime().asSeconds();
				}
			}
		}
		);
	}
}
