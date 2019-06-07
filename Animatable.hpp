#pragma once
#include <vector>
#include "GameObject.hpp"

namespace rstar
{
	class Animatable : public GameObject
	{
	public:
		Animatable(GameDataPtr data, std::vector<sf::Texture> const& texturesInOrder, float frameTime, sf::Clock const& clock);

		void ChangeTextures(std::vector<sf::Texture> const& textures);

	protected:
		const std::vector<sf::Texture> * textures_;
		float frameTime_;
		sf::Clock const& clockRef_;

		unsigned currentAnimationTexture_{ 0 };
		float animationTimeOffset_{ 0.f };

		void animate();
	};
}
