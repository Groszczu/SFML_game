#include "InteractionsHandler.hpp"
#include "Enemy.hpp"
#include "PlayerShip.hpp"

namespace rstar
{
	void InteractionsHandler::Run(Enemies &e, PlayerShip &ship)
	{
		bullets(e, ship);
		bodiesIntersection(e, ship);
		enemiesShooting(e, ship);
	}

	void InteractionsHandler::bullets(Enemies &e, PlayerShip &ship)
	{
		for (auto &enemy : e.enemies_)
		{
			ship.bullets_.erase(std::remove_if(begin(ship.bullets_), end(ship.bullets_),
				[&](auto &bullet)
				{
					auto hit = bullet->GetBounds().intersects(enemy->GetBounds()) && !enemy->IsDestroyed();
					if (hit)
					{
						enemy->isDestroyed_ = true;
						enemy->isCharging_ = false;
						ship.score_ += LVL1_POINTS_FOR_ENEMY;
					}

					return hit;
				}
			), end(ship.bullets_));

			e.bullets_.erase(std::remove_if(begin(e.bullets_), end(e.bullets_),
				[&](auto &bullet)
				{
					auto hit = bullet->GetBounds().intersects(ship.GetBounds());
					if (hit)
					{
						--ship.lives_;
					}

					return hit;
				}
			), end(e.bullets_));
		}
	}

	void InteractionsHandler::bodiesIntersection(Enemies &e, PlayerShip &ship)
	{
		for (auto &enemy : e.enemies_)
		{
			if (!enemy->IsDestroyed() && enemy->GetBounds().intersects(ship.GetBounds()))
			{
				enemy->isDestroyed_ = true;
				enemy->isCharging_ = false;
				--ship.lives_;
			}

			if (enemy->IsOutOfScreen())
			{
				enemy->toRemove_ = true;
				ship.score_ -= LVL1_POINTS_FOR_ENEMY;
			}
		}
	}

	void InteractionsHandler::enemiesShooting(Enemies &e, PlayerShip& ship)
	{
		for (auto &enemy : e.enemies_)
		{
			if (!enemy->IsCharging()
				&& !enemy->IsDestroyed()
				&& e.lvlClockRef_.getElapsedTime().asSeconds() > LVL1_ENEMIES_START_SHOOT_DELAY
				&& e.lvlClockRef_.getElapsedTime().asSeconds() - e.shotDelayTimeOffset_ > ENEMIES_SHOT_DELAY
				&& abs(enemy->GetPosition().x - ship.GetPosition().x) < ENEMIES_WIDTH)
			{
				if (Random<float>(0, 100) < LVL1_ENEMIES_CHANCE_TO_SHOOT)
				{
					e.Shoot(sf::Vector2f{ enemy->GetPosition().x - enemy->GetBounds().width / 2.f, enemy->GetPosition().y });
					e.shotDelayTimeOffset_ = e.lvlClockRef_.getElapsedTime().asSeconds();
				}
			}
		}
	}
}
