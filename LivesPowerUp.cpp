#include "LivesPowerUp.hpp"

namespace rstar
{
	LivesPowerUp::LivesPowerUp(GameDataPtr data, float movementSpeed, Direction startMoveDirection, unsigned lives, sf::Clock &clock)
		: PowerUpShip(data, movementSpeed, startMoveDirection, clock), lives_(lives)
	{
		SetTexture(data_->assets.GetTexture("Lives PowerUp"));
		sprite_.setScale(2.f, 2.f);
		sprite_.setPosition(startMoveDirection == left ? sf::Vector2f{ WINDOW_WIDTH + ENEMIES_WIDTH, 2 * ENEMIES_HEIGHT } :
			sf::Vector2f{ -ENEMIES_WIDTH, 2 * ENEMIES_HEIGHT });
	}

	void LivesPowerUp::PowerUp(PlayerShip& ship)
	{
		ship.IncreaseLives(lives_);
	}

	void LivesPowerUp::Update()
	{
		if (moveDirection_ == left && GetPosition().x < - GetBounds().width
			|| moveDirection_ == right && GetPosition().x > WINDOW_WIDTH + GetBounds().width)
		{
			toRemove_ = true;
		}

		if (isDestroyed_)
		{
			animateDestroy();
		}
		else if(!toRemove_)
		{
			animate();
			sprite_.move(moveDirection_ * movementSpeed_, 0);
		}
	}

	void LivesPowerUp::animate()
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

	void LivesPowerUp::animateDestroy()
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

	void LivesPowerUp::Draw() const
	{
		data_->window.draw(sprite_);
	}
}
