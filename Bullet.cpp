#include "Bullet.hpp"

namespace rstar
{
	BaseBullet::Bullet::Bullet(GameDataPtr data, sf::Vector2f startPosition) : GameObject(data)
	{
		sprite_.setTexture(data_->assets.GetTexture("Player Bullet"));
		sprite_.setScale(3.f, 3.f);
		sprite_.setPosition(startPosition);
	}

	BaseBullet::Bullet::~Bullet()
	{
		data_.reset();
	}

	bool BaseBullet::Bullet::IsOutOfScreen() const
	{
		return GetPosition().y < 0 - GetHeight();
	}

	void BaseBullet::Bullet::Update()
	{
		sprite_.move({ 0.f, -movementSpeed_ });
	}

	void BaseBullet::Bullet::Draw() const
	{
		data_->window.draw(sprite_);
	}
}