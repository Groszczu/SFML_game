#include "DEFINITIONS.hpp"
#include "GameObject.hpp"
#include "Enemy.hpp"
#include <algorithm>
#include <utility>

namespace rstar
{
	Direction Enemies::MoveDirection = right;
	float Enemies::MovementSpeed = ENEMIES_START_MOVEMENT_SPEED;
	bool Enemies::MoveForward = false;
	float Enemies::BulletsSpeed = LVL1_ENEMIES_BULLETS_SPEED;
	float Enemies::ChargingSpeed = LVL1_ENEMIES_CHARGING_SPEED;

	// Start enemy---------------------------------------------------------------------------
	Enemy::Enemy(GameDataPtr data, sf::Vector2f startPosition, sf::Clock &clock)
		: GameObject(data), clockRef_(clock)
	{
		SetTexture(data_->assets.GetTexture("Red Enemy1"));
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
			case 1: SetTexture(data_->assets.GetTexture("Red Enemy2"));
				++currentTexture_;
				break;
			case 2: SetTexture(data_->assets.GetTexture("Red Enemy3"));
				++currentTexture_;
				break;
			case 3: SetTexture(data_->assets.GetTexture("Red Enemy2"));
				++currentTexture_;
				break;
			case 4: SetTexture(data_->assets.GetTexture("Red Enemy1"));
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
			case 0: SetTexture(data_->assets.GetTexture("Enemy dst1"));
				++currentDestroyTexture_;
				break;
			case 1: SetTexture(data_->assets.GetTexture("Enemy dst2"));
				++currentDestroyTexture_;
				break;
			case 2: SetTexture(data_->assets.GetTexture("Enemy dst3"));
				++currentDestroyTexture_;
				break;
			case 3: SetTexture(data_->assets.GetTexture("Enemy dst4"));
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
		sf::Vector2f moveDirection;
		if (!isCharging_)
		{
			if (GetPosition().x > WINDOW_WIDTH)
			{
				Enemies::MoveDirection = left;
			}
			if (GetPosition().x - ENEMIES_WIDTH < 0)
			{
				Enemies::MoveDirection = right;
			}

			moveDirection = { Enemies::MoveDirection * Enemies::MovementSpeed, Enemies::MoveForward * Enemies::MovementSpeed };

		}
		else
		{
			moveDirection = { Enemies::MoveDirection * Enemies::MovementSpeed, Enemies::ChargingSpeed };
		}
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

		MoveDirection = right;
		MovementSpeed = ENEMIES_START_MOVEMENT_SPEED;
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
			handleCharging(LVL1_ENEMIES_CHARGING_AT_ONCE);
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
		reorderEnemies();
	}

	void Enemies::Draw() const
	{
		for (auto const& enemy : enemies_)
		{
			enemy->Draw();
		}

		for (auto const& bullet : bullets_)
		{
			bullet->Draw();
		}
	}

	void Enemies::Shoot(sf::Vector2f const& startPosition)
	{
		// putting new Bullet object in the bullets_ vector [as new Bullet unique_ptr]
		bullets_.emplace_back(std::make_unique<Bullet>(data_, startPosition, BulletsSpeed));
	}

	void Enemies::handleCharging(unsigned int enemiesCharging)
	{
		for (unsigned int i = 0; i < enemiesCharging; i++)
		{
			auto const chargingEnemyIndex = Random<int>(0, GetEnemiesCount() - 1);
			enemies_.at(chargingEnemyIndex)->Charge();
		}
	}

	// reorder enemies to draw them in correct order`
	void Enemies::reorderEnemies()
	{
		std::partition(begin(enemies_), end(enemies_),
			[](auto const& enemy)
			{
				return enemy->IsCharging();
			}
		);

		std::stable_partition(begin(enemies_), end(enemies_),
			[](auto const& enemy)
			{
				return !enemy->IsDestroyed() && !enemy->IsCharging();
			}
		);
	}

	// End enemies---------------------------------------------------------------------

	// Free functions------------------------------------------------------------------
	void HandleBulletsIntersection(Enemies &e, PlayerShip &ship)
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
						enemy->isCharging_ = false;
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
	void HandleIntersection(Enemies &e, PlayerShip &ship)
	{
		std::for_each(begin(e.enemies_), end(e.enemies_),
			[&](auto &enemy)
			{
				if (!enemy->IsDestroyed() && enemy->GetBounds().intersects(ship.GetBounds()))
				{
					enemy->isDestroyed_ = true;
					enemy->isCharging_ = false;
					--ship.lives_;
				}

				if (enemy->IsOutOfScreen())
				{
					enemy->toRemove_ = true;
					ship.score_ -= LVL1_POINTS_FOR_ENEMY;
				}
			}
		);
	}

	void HandleEnemiesShooting(Enemies &e, PlayerShip &ship)
	{
		std::for_each(begin(e.enemies_), end(e.enemies_),
			[&e, &ship](auto &enemy)
			{
				if (!enemy->IsCharging()
					&& e.lvlClockRef_.getElapsedTime().asSeconds() > LVL1_ENEMIES_START_SHOOT_DELAY
					&& e.lvlClockRef_.getElapsedTime().asSeconds() - e.shotDelayTimeOffset_ > ENEMIES_SHOT_DELAY
					&& enemy->GetPosition().x + ENEMIES_WIDTH > ship.GetPosition().x
					&& enemy->GetPosition().x - ENEMIES_WIDTH < ship.GetPosition().x)
				{
					if (Random<float>(0, 100) < LVL1_ENEMIES_CHANCE_TO_SHOOT)
					{
						e.Shoot(sf::Vector2f{ enemy->GetPosition().x - enemy->GetBounds().width / 2.f, enemy->GetPosition().y });
						e.shotDelayTimeOffset_ = e.lvlClockRef_.getElapsedTime().asSeconds();
					}
				}
			}
		);
	}
}
