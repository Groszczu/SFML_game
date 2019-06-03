#include "PowerUpShip.hpp"


namespace rstar
{
	PowerUpShip::PowerUpShip(GameDataPtr data, float movementSpeed, DirectionX startMoveDirection, unsigned powerUpValue, 
		std::vector<sf::Texture> const& textures, float frameTime, sf::Clock const& clock)
		: Animatable(data, textures, frameTime, clock), movementSpeed_(movementSpeed), moveDirection_(startMoveDirection), powerUpValue_(powerUpValue)
	{
		sprite_.setScale(2.f, 2.f);
		sprite_.setRotation(180.f);
		sprite_.setPosition(startMoveDirection == DirectionX::left ? sf::Vector2f{ WINDOW_WIDTH + ENEMIES_WIDTH, 2 * ENEMIES_HEIGHT } :
			sf::Vector2f{ -ENEMIES_WIDTH, 2 * ENEMIES_HEIGHT });
	}

	void PowerUpShip::Update()
	{
		if (!toRemove_)
		{
			if (moveDirection_ == DirectionX::left && GetPosition().x < -GetBounds().width ||
				moveDirection_ == DirectionX::right && GetPosition().x > WINDOW_WIDTH + GetBounds().width)
			{
				toRemove_ = true;
			}

			if (isDestroyed_)
			{
				animateDestroy();
			}
			else
			{
				animate();
				sprite_.move(static_cast<int>(moveDirection_) * movementSpeed_, 0);
			}
		}
	}

	void PowerUpShip::animateDestroy()
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

	void PowerUpShip::Draw() const
	{
		data_->window.draw(sprite_);
	}
}
