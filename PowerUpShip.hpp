#pragma once
#include "GameObject.hpp"
#include "Enemy.hpp"

namespace rstar
{
	class PowerUpShip : public GameObject
	{
	public:
		PowerUpShip(GameDataPtr data, float movementSpeed, Direction startMoveDirection, sf::Clock &clock);

		bool IsToRemove() const { return toRemove_; }
		bool IsDestroyed() const { return isDestroyed_; }

		virtual void PowerUp(PlayerShip &ship) = 0;

		void Update() override = 0;
		void Draw() const override = 0;

		friend class InteractionsHandler;

	protected:
		float movementSpeed_;
		const Direction moveDirection_;
		sf::Clock &clockRef_;

		float baseAnimationTimeOffset_{ 0.f };
		float destroyAnimationTimeOffset_{ 0.f };

		unsigned short currentTexture_{ 1 };
		unsigned short currentDestroyTexture_{ 0 };

		bool isDestroyed_{ false };
		bool toRemove_{ false };
	};
}

