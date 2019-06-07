#include "DEFINITIONS.hpp"
#include "GameObject.hpp"
#include "Enemy.hpp"
#include <algorithm>
#include <utility>

namespace rstar
{
	DirectionX Enemies::MoveDirection{};
	float Enemies::MovementSpeed{};
	bool Enemies::MoveForward{};
	float Enemies::BulletsSpeed{};
	float Enemies::ChargingSpeed{};

	// Start enemy---------------------------------------------------------------------------
	Enemy::Enemy(GameDataPtr data, std::vector<sf::Texture> const& textures, sf::Vector2f startPosition, unsigned lives,
		float frameTime, sf::Clock const& clock)
		: Animatable(data, textures, frameTime, clock), lives_(lives)
	{
		updateTextures();
		sprite_.setScale(2.f, 2.f);
		sprite_.setRotation(180);
		sprite_.setPosition(startPosition);
	}

	void Enemy::animateDestroy()
	{
		if (clockRef_.getElapsedTime().asSeconds() - destroyAnimationTimeOffset_ > BOOM_ANIMATION_FRAME_TIME)
		{
			if (currentDestroyTexture_ < data_->assets.GetTexturesArray("Enemy dst").size())
			{
				SetTexture(data_->assets.GetTexture("Enemy dst", currentDestroyTexture_++));
				destroyAnimationTimeOffset_ = clockRef_.getElapsedTime().asSeconds();
			}
			else
			{
				toRemove_ = true;
			}		
		}
	}

	void Enemy::handleMovement()
	{
		sf::Vector2f moveDirection;

		if (GetPosition().x > WINDOW_WIDTH)
		{
			Enemies::MoveDirection = DirectionX::left;
		}
		if (GetPosition().x - GetBounds().width < 0)
		{
			Enemies::MoveDirection = DirectionX::right;
		}

		if (!isCharging_)
		{
			if (GetPosition().y > WINDOW_HEIGHT - 3 * GetBounds().height)
			{
				Charge();
				return;
			}

			moveDirection = { static_cast<int>(Enemies::MoveDirection) * Enemies::MovementSpeed, Enemies::MoveForward * Enemies::MovementSpeed };

		}
		else
		{
			moveDirection = { static_cast<int>(Enemies::MoveDirection) * Enemies::MovementSpeed, Enemies::ChargingSpeed };
		}
		sprite_.move(moveDirection);
	}

	void Enemy::Update()
	{
		if (isDestroyed_)
		{
			animateDestroy();
		}
		else
		{
			if (hit_)
			{
				updateTextures();
			}
			animate();
			handleMovement();
		}
	}

	void Enemy::updateTextures()
	{
		switch (GetLives())
		{
		case 1:
			ChangeTextures(data_->assets.GetTexturesArray("Red Enemy"));
			break;
		case 2:
			ChangeTextures(data_->assets.GetTexturesArray("Blue Enemy"));
			break;
		case 3:
			ChangeTextures(data_->assets.GetTexturesArray("Gold Enemy"));
			break;
		default:
			break;
		}
	}


	void Enemy::Draw() const
	{
		data_->window.draw(sprite_);

	}
	// End enemy---------------------------------------------------------------------------

	// Start enemies-----------------------------------------------------------------------
	Enemies::Enemies(GameDataPtr data, unsigned enemiesCount, float movementSpeed, float bulletsSpeed, float chargingSpeed, unsigned enemiesCharging,
		unsigned enemiesLives, sf::Vector2f firstEnemyPos, float space, sf::Clock const& lvlClockRef)
		: data_(std::move(data)), lvlClockRef_(lvlClockRef), enemiesCount_(enemiesCount), enemiesCharging_(enemiesCharging)
	{
		std::generate_n(std::back_inserter(enemies_), enemiesCount,
			[&]
			{
				if (firstEnemyPos.x + ENEMIES_WIDTH >= WINDOW_WIDTH - ENEMIES_SIDE_MARGIN)
				{
					firstEnemyPos.x = ENEMIES_SIDE_MARGIN;
					firstEnemyPos.y += ENEMIES_HEIGHT + space;
				}

				firstEnemyPos.x += ENEMIES_WIDTH + space;

				return std::make_unique<Enemy>(data_, data_->assets.GetTexturesArray("Red Enemy"), firstEnemyPos, enemiesLives, 
					ENEMY_ANIMATION_FRAME_TIME, lvlClockRef_);
			}
			);

		MoveDirection = DirectionX::right;
		MovementSpeed = movementSpeed;
		BulletsSpeed = bulletsSpeed;
		ChargingSpeed = chargingSpeed;
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

	void Enemies::removeMissedBullets()
	{
		bullets_.erase(std::remove_if(begin(bullets_), end(bullets_),
			[](auto &bullet)
		{
			return bullet->IsOutOfScreen();
		}), end(bullets_));
	}


	void Enemies::Update()
	{
		if (lvlClockRef_.getElapsedTime().asSeconds() - moveForwardTimeOffset_ > ENEMIES_MOVE_FORWARD_TIME && !MoveForward)
		{
			MoveForward = true;
			moveForwardTimeOffset_ = lvlClockRef_.getElapsedTime().asSeconds();
			handleCharging(enemiesCharging_);
		}
		if (MoveForward && lvlClockRef_.getElapsedTime().asSeconds() - moveForwardTimeOffset_ > ENEMIES_MOVE_FORWARD_DURATION)
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
		removeMissedBullets();
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
		// putting new Bullet object in the bullet_ vector [as new Bullet unique_ptr]
		bullets_.emplace_back(std::make_unique<Bullet>(data_, data_->assets.GetTexture("Enemy Bullet"), startPosition, BulletsSpeed));
	}

	void Enemies::handleCharging(unsigned int enemiesCharging)
	{
		if (!enemies_.empty())
		{
			while(enemiesCharging--)
			{
				enemies_.at(Random<int>(0, GetEnemiesCount() - 1))->Charge();
			}
		}
		
	}

	// reorder enemies to draw them in correct order
	// drawing not destroyed and not charging first
	// then drawing charging enemies
	// and then destroyed
	// displaying order is reversed
	void Enemies::reorderEnemies()
	{
		// put destroyed enemies on the end of std::vector (reverse iterators)
		auto const firstNotDestroyed = std::partition(rbegin(enemies_), rend(enemies_),
			[](auto const& enemy)
			{
				return enemy->IsDestroyed();
			}
		);

		// put charging enemies just before destroyed
		std::stable_partition(firstNotDestroyed, rend(enemies_),
			[](auto const& enemy)
			{
				return enemy->IsCharging();
			}
		);
	}
	// End enemies---------------------------------------------------------------------
}
