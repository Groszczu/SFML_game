#include "PlayerShip.hpp"
#include "DEFINITIONS.hpp"
#include <vector>

namespace rstar
{
	PlayerShip::PlayerShip(GameDataPtr data) : GameObject(data)
	{
		sprite_.setTexture(data_->assets.GetTexture("Player Ship"));
		sprite_.setScale(2.f, 2.f);
		sprite_.setPosition(static_cast<float>(WINDOW_WIDTH) / 2.f - GetBounds().width / 2.f, static_cast<float>(WINDOW_HEIGHT) - GetBounds().height * 2.f);
	}

	void PlayerShip::Shoot()
	{
		// putting new Bullet object in the bullets_ vector [as new Bullet unique_ptr]
		bullets_.emplace_back(std::make_unique<Bullet>(data_, sf::Vector2f{ GetPosition().x + GetBounds().width / 2.f, GetPosition().y }, bulletsSpeed));
	}

	void PlayerShip::Update()
	{
		sf::Vector2f movement{0,0};

		// if A is pressed change movement vector.x to negative value
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && GetPosition().x > 0)
		{
			movement.x = -movementSpeed_;
		}

		// if D is pressed change movement vector.x to positive value
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && GetPosition().x + GetBounds().width < WINDOW_WIDTH)
		{
			movement.x = movementSpeed_;
		}

		// if space is pressed shoot
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shotClock.getElapsedTime().asSeconds() > SHOT_DELAY)
		{
			Shoot();
			shotClock.restart();
		}

		// Updating bullets position
		for (std::size_t i = 0; i < bullets_.size(); i++)
		{
			if (bullets_.at(i)->IsOutOfScreen())
			{
				// removing bullets that are out of screen
				bullets_.erase(bullets_.begin() + i);
			}
			else
			{
				// if bullet position is valid, move it
				bullets_.at(i)->Update();
			}
		}

		// move player ship
		sprite_.move(movement);
	}

	// drawing player ship and its bullets to the screen
	void PlayerShip::Draw() const
	{
		data_->window.draw(sprite_);
		for (auto const& bullet : bullets_)
		{
			bullet->Draw();
		}
	}

}
