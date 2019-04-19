#pragma once
#include "GameObject.hpp"
#include "Game.hpp"

namespace rstar
{
	// class to inherit from
	// derived class inherits protected class Bullet as its member
	// [this allows to reuse Bullet class in many classes]
	class BaseBullet
	{
	protected:
		class Bullet : public GameObject
		{
		public:
			Bullet(GameDataPtr data, sf::Vector2f startPosition, float movementSpeed);

			bool IsOutOfScreen() const;

			void Update() override;
			void Draw() const override;

		private:
			float movementSpeed_{ 10.f };
		};
	};
}
