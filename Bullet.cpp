#include "Bullet.hpp"

namespace rstar
{
	BaseBullet::Bullet::Bullet(GameDataPtr data, sf::Vector2f startPosition, float movementSpeed, bool playerBullet)
		: GameObject(data), movementSpeed_(movementSpeed)
	{
		if (playerBullet)
		{
			sprite_.setTexture(data_->assets.GetTexture("Player Bullet"));
		}
		else
		{
			sprite_.setTexture(data_->assets.GetTexture("Enemy Bullet"));
		}
		sprite_.setScale(3.f, 3.f);
		sprite_.setPosition(startPosition);
	}

	void BaseBullet::Bullet::Update()
	{
		sprite_.move({ 0.f, movementSpeed_ });
	}

	void BaseBullet::Bullet::Draw() const
	{
		data_->window.draw(sprite_);
	}
}
