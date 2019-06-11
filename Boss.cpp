#include "Boss.hpp"

namespace rstar
{
	Boss::Boss(GameDataPtr data, std::vector<sf::Texture> const& textures, 
		unsigned lives, float movementSpeed, float chargingSpeed, float bulletsSpeed,
		sf::Vector2f const startPosition, float frameTime, sf::Clock const& clock)
		: Enemy(data, textures, startPosition, lives, frameTime, clock),
		movementSpeed_(movementSpeed), chargingSpeed_(chargingSpeed), startPosition_(startPosition), bulletsSpeed_(bulletsSpeed)
	{
		hpBar_.setSize({ HP_BAR_LENGTH_PER_HP * lives_, HP_BAR_HEIGHT });
		hpBar_.setPosition(startPosition_);
		hpBar_.setFillColor(sf::Color(242, 38, 19, 100));
		sprite_.setScale(4.f, 4.f);
	}

	void Boss::Update()
	{
		if (isDestroyed_)
		{
			animateDestroy();
		}
		else
		{
			if (hit_)
			{
				animateHpBar();
			}
			animateLaserLoading();
			handleMovement();
			handleCharging();
			handleShooting();
		}

		for (auto &bullet : bullets_)
		{
			bullet->Update();
		}

		updateHpBar();
	}

	void Boss::Charge()
	{
		positionOffset_ = GetPosition().y >= WINDOW_HEIGHT - 2*GetBounds().height ? startPosition_.y : GetPosition().y;
		shootingCounter_ = 0;
		isCharging_ = true;
	}

	void Boss::updateHpBar()
	{
		hpBar_.setSize({ HP_BAR_LENGTH_PER_HP * lives_, hpBar_.getGlobalBounds().height });
		hpBar_.setPosition(sprite_.getPosition());
	}


	void Boss::handleMovement()
	{
		if (GetPosition().y - GetBounds().height > WINDOW_HEIGHT)
		{
			sprite_.setPosition(GetPosition().x, -GetBounds().height);
		}

		sf::Vector2f moveVector;

		if (GetPosition().x > WINDOW_WIDTH)
		{
			moveDirection_ = DirectionX::left;
		}
		if (GetPosition().x - GetBounds().width < 0)
		{
			moveDirection_ = DirectionX::right;
		}

		if (!isCharging_)
		{
			moveVector = { static_cast<int>(moveDirection_) * movementSpeed_, 0 };
		}
		else
		{
			moveVector = { static_cast<int>(moveDirection_) * movementSpeed_, chargingSpeed_ };
			if (abs(GetPosition().y - BOSS_MOVE_FORWARD_PER_CHARGE - positionOffset_) <= 1)
			{
				if(chargesCounter_-- == 0)
				{
					isCharging_ = false;
					positionOffset_ = 0.f;
					chargesCounter_ = BOSS_CHARGING_NUMBER;
				}
			}
		}
		sprite_.move(moveVector);
	}

	void Boss::handleCharging()
	{
		if (clockRef_.getElapsedTime().asSeconds() - chargeTimeOffset_ >= BOSS_CHARGING_DELAY)
		{
			Charge();
			chargeTimeOffset_ = clockRef_.getElapsedTime().asSeconds();
		}
	}

	void Boss::handleShooting()
	{
		if (!isCharging_ && clockRef_.getElapsedTime().asSeconds() - shootTimeOffset_ >= BOSS_SHOOTING_DELAY)
		{
			if (shootingCounter_ >= BOSS_SHOTS_UNTIL_LASER)
			{
				sf::Vector2f shootPosition{ GetPosition().x - GetBounds().width / 2 - 18.f, GetPosition().y - GetBounds().height / 2 };
				bullets_.emplace_back(std::make_unique<Bullet>(data_, data_->assets.GetTexture("Boss Laser"),
					shootPosition, 16 * bulletsSpeed_, DirectionX::none, DirectionY::down));

				if (++shootingCounter_ > BOSS_LASER_SHOTS)
				{
					shootingCounter_ = 0;
				}
			}
			else
			{
				sf::Vector2f shootPosition{ GetPosition().x - GetBounds().width / 2 - 18.f, GetPosition().y - GetBounds().height / 2 };
				bullets_.emplace_back(std::make_unique<Bullet>(data_, data_->assets.GetTexture("Boss Bullet"),
					shootPosition, bulletsSpeed_, DirectionX::left, DirectionY::down));
				bullets_.emplace_back(std::make_unique<Bullet>(data_, data_->assets.GetTexture("Boss Bullet"),
					shootPosition, bulletsSpeed_, DirectionX::none, DirectionY::down));
				bullets_.emplace_back(std::make_unique<Bullet>(data_, data_->assets.GetTexture("Boss Bullet"),
					shootPosition, bulletsSpeed_, DirectionX::right, DirectionY::down));

				++shootingCounter_;
				shootTimeOffset_ = clockRef_.getElapsedTime().asSeconds();
			}
		}

		bullets_.erase(std::remove_if(begin(bullets_), end(bullets_),
			[](auto &bullet)
		{
			return bullet->IsOutOfScreen();
		}), end(bullets_));
	}

	void Boss::animateLaserLoading()
	{
		if (shootingCounter_ >= BOSS_SHOTS_UNTIL_LASER &&
			currentAnimationTexture_ < textures_->size() - 1)
		{
			animate();
		}
		else if (shootingCounter_ == 0)
		{
			SetTexture(textures_->at(0));
			currentAnimationTexture_ = 0;
		}
	}

	void Boss::animateHpBar()
	{
		if (clockRef_.getElapsedTime().asSeconds() - hpBarAnimationTimeOffset_ > HP_BAR_ANIMATION_FRAME_TIME)
		{
			if (hpBar_.getFillColor() == sf::Color(242, 38, 19, 100))
			{
				hpBar_.setFillColor(sf::Color(240, 255, 0, 100));
			}
			else
			{
				hpBar_.setFillColor(sf::Color(242, 38, 19, 100));
			}

			hpBarAnimationTimeOffset_ = clockRef_.getElapsedTime().asSeconds();

			if (++hpBarAnimationFrame_ >= HP_BAR_ANIMATION_FRAMES)
			{
				hpBarAnimationFrame_ = 0;
				hpBar_.setFillColor(sf::Color(242, 38, 19, 100));
				hit_ = false;
			}
		}
	}

	void Boss::Draw() const
	{
		Enemy::Draw();
		data_->window.draw(hpBar_);
		for (auto const& bullet : bullets_)
		{
			bullet->Draw();
		}
	}
}
