#pragma once
#include <vector>
#include "GameObject.hpp"

namespace rstar
{
	class Animatable : public GameObject
	{
	public:
		Animatable(GameDataPtr data, std::vector<sf::Texture> texturesInOrder, float frameTime, sf::Clock const& clock);

	protected:
		std::vector<sf::Texture> const textures_;
		float frameTime_;
		sf::Clock const& clockRef_;

		unsigned currentAnimationTexture_{ 0 };
		float animationTimeOffset_{ 0.f };

		void animate();
	};
}
