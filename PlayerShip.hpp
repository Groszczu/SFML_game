#pragma once
#include <vector>
#include <memory>
#include "GameObject.hpp"
#include "Bullet.hpp"

namespace rstar
{
	class Enemies;
	class PlayerShip final : public GameObject, public BaseBullet
	{
	public:
		explicit PlayerShip(GameDataPtr data);

		void Shoot();
		std::vector<std::unique_ptr<Bullet>>* GetBulletsPtr() { return &bullets_; }

		void Update() override;
		void Draw() const override;

		friend void RemovePlayerBullet(Enemies &enemies, PlayerShip &playerShip);
		
	private:
		float movementSpeed_{ 2.5f };
		float bulletsSpeed{ -10.f }; // minus due to destination of bullets
		std::vector<std::unique_ptr<Bullet>> bullets_;

		sf::Clock shotClock;
	};
}
