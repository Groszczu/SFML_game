#pragma once
#include "Enemy.hpp"

namespace rstar
{
	class Boss : public Enemy
	{
	public:
		Boss(GameDataPtr data, std::vector<sf::Texture> const& textures, sf::Vector2f startPosition, unsigned lives,
			float frameTime, sf::Clock const& clock);

		void Update() override;
		void Draw() const override;

	private:
		void handleMovement() override;
	};
}

