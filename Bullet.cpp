#include "Bullet.hpp"

namespace rstar
{
	Bullet::Bullet(GameDataPtr data, sf::Texture const& texture, sf::Vector2f startPosition, float movementSpeed, DirectionX directionX, DirectionY directionY)
		: GameObject(data), movementSpeed_(movementSpeed), directionX_(directionX), directionY_(directionY)
	{
		sprite_.setTexture(texture);
		sprite_.setScale(3.f, 3.f);
		sprite_.setPosition(startPosition);
	}

	void Bullet::Update()
	{
		sprite_.move({ static_cast<int>(directionX_) * movementSpeed_, static_cast<int>(directionY_) * movementSpeed_ });
	}

	void Bullet::Draw() const
	{
		data_->window.draw(sprite_);
	}
}
