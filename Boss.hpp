#pragma once
#include "Enemy.hpp"

namespace rstar
{
	class Boss : public Enemy, public BaseBullet
	{
	public:
		Boss(GameDataPtr data, std::vector<sf::Texture> const& textures, 
			unsigned lives, float movementSpeed, float chargingSpeed, float bulletsSpeed,
			sf::Vector2f startPosition, float frameTime, sf::Clock const& clock);

		void Charge() override;
		void Update() override;
		void Draw() const override;

		friend class InteractionsHandler;

	private:
		float movementSpeed_;

		sf::RectangleShape hpBar_;

		float chargingSpeed_;
		unsigned chargingCounter_{ BOSS_CHARGING_NUMBER };
		float chargeTimeOffset_{ 0.f };

		float positionOffset_{ 0.f };
		sf::Vector2f const startPosition_;

		float shootTimeOffset_{ 0.f };
		unsigned shootingCounter_{ 0 };
		float bulletsSpeed_;
		std::vector<std::unique_ptr<Bullet>> bullets_;

		DirectionX moveDirection_{ DirectionX::right };

		void handleMovement() override;
		void handleCharging();
		void handleShooting();
		void animateLaserLoading();

		float hpBarAnimationTimeOffset_{ 0.f };
		unsigned hpBarAnimationFrame_{ 0 };
		void animateHpBar();
		void updateHpBar();
	};
}

