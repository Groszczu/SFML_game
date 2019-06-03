#pragma once
#include "GameObject.hpp"
#include "Enemy.hpp"
#include "Animatable.hpp"

namespace rstar
{
	class PowerUpShip : public Animatable
	{
	public:
		PowerUpShip(GameDataPtr data, float movementSpeed, DirectionX startMoveDirection, unsigned powerUpValue, 
			std::vector<sf::Texture> const& textures, float frameTime, sf::Clock const& clock);

		bool IsToRemove() const { return toRemove_; }
		bool IsDestroyed() const { return isDestroyed_; }

		virtual void PowerUp(PlayerShip &ship) = 0;

		void Update() override;
		void Draw() const override;

		friend class InteractionsHandler;

	protected:
		float movementSpeed_;
		const DirectionX moveDirection_;
		unsigned powerUpValue_;

		bool isDestroyed_{ false };
		bool toRemove_{ false };

		float destroyAnimationTimeOffset_{ 0.f };
		unsigned short currentDestroyTexture_{ 0 };
		void animateDestroy();
	};
}

