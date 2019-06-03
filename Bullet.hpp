#pragma once
#include "GameObject.hpp"
#include "Game.hpp"

namespace rstar
{
	enum class DirectionY
	{
		up = -1,
		down = 1
	};

	// class to inherit from
	// derived class inherits protected class Bullet as its member
	// [this allows to reuse Bullet class in many classes]
	class BaseBullet
	{
	protected:
		class Bullet : public GameObject
		{
		public:
			Bullet(GameDataPtr data, sf::Vector2f startPosition, float movementSpeed, bool playerBullet = false);

			void Update() override;
			void Draw() const override;

		private:
			float movementSpeed_;
			DirectionY direction_;
		};
	};
}
