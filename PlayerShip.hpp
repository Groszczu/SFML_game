#pragma once
#include <vector>
#include <memory>
#include "GameObject.hpp"
#include "Bullet.hpp"

namespace rstar
{
	class PlayerShip final : public GameObject, public BaseBullet
	{
	public:
		explicit PlayerShip(GameDataPtr data);
		~PlayerShip();

		void Shoot();

		void Update() override;
		void Draw() const override;

	private:
		float movementSpeed_{ 2.5f };
		std::vector<std::unique_ptr<Bullet>> bullets_;

		sf::Clock shotClock;
	};
}
