#include "PlayerShip.hpp"
#include "DEFINITIONS.hpp"
#include <vector>

namespace rstar
{
	PlayerShip::PlayerShip(GameDataPtr data, sf::Clock &clockRef)
		: GameObject(data), shotClockRef_(clockRef)
	{
		sprite_.setTexture(data_->assets.GetTexture("Player Ship"));
		sprite_.setScale(2.f, 2.f);
		sprite_.setPosition(static_cast<float>(WINDOW_WIDTH) / 2.f - GetBounds().width / 2.f, static_cast<float>(WINDOW_HEIGHT) - GetBounds().height * 2.f);
	}

	void PlayerShip::Shoot()
	{
		// putting new Bullet object in the bullets_ vector [as new Bullet unique_ptr]
		bullets_.emplace_back(std::make_unique<Bullet>(data_, sf::Vector2f{ GetPosition().x + GetBounds().width / 2.f, GetPosition().y }, bulletsSpeed_));
	}

	void PlayerShip::Update()
	{
		if (lives_ != 0)
		{
			handleMovement();
			handleShooting();
		}
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

	void PlayerShip::handleMovement()
	{
		sf::Vector2f moveDirection{0,0};

		// if A is pressed change movement vector.x to negative value
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && GetPosition().x > 0)
		{
			moveDirection.x = -movementSpeed_;
		}

		// if D is pressed change movement vector.x to positive value
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && GetPosition().x + GetBounds().width < WINDOW_WIDTH)
		{
			moveDirection.x = movementSpeed_;
		}

		// move player ship
		sprite_.move(moveDirection);
	}

	void PlayerShip::handleShooting()
	{
		// if space is pressed shoot
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) 
			&& shotClockRef_.getElapsedTime().asSeconds() - shotDelayTimeOffset_ > SHOT_DELAY)
		{
			Shoot();
			shotDelayTimeOffset_ = shotClockRef_.getElapsedTime().asSeconds();
		}

		// Updating bullets position
		bullets_.erase(std::remove_if(begin(bullets_), end(bullets_),
			[](auto &bullet)
		{
				return bullet->IsOutOfScreen();
		}
		), end(bullets_));

		for (auto &bullet : bullets_)
		{
			bullet->Update();
		}
	}
}
