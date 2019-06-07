#include "Boss.hpp"

namespace rstar
{
	Boss::Boss(GameDataPtr data, std::vector<sf::Texture> const& textures, sf::Vector2f startPosition, unsigned lives,
		 float frameTime, sf::Clock const& clock)
		: Enemy(data, textures, startPosition, lives, frameTime, clock)
	{
		updateTextures();
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
			animate();
			handleMovement();
		}
	}

	void Boss::handleMovement()
	{
		if (GetPosition().y - GetBounds().height > WINDOW_HEIGHT)
		{
			sprite_.setPosition(GetPosition().x, -GetBounds().height);
		}

		Enemy::handleMovement();
	}

	void Boss::Draw() const
	{
		Enemy::Draw();
	}
}
