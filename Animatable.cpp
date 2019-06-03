#include "Animatable.hpp"
#include <utility>


namespace rstar
{
	Animatable::Animatable(GameDataPtr data, std::vector<sf::Texture> texturesInOrder, float frameTime, sf::Clock const& clock)
		: GameObject(data), textures_(std::move(texturesInOrder)), frameTime_(frameTime), clockRef_(clock)
	{
	}

	void Animatable::animate()
	{
		if (clockRef_.getElapsedTime().asSeconds() - animationTimeOffset_ > frameTime_)
		{
			if (currentAnimationTexture_ >= textures_.size())
			{
				currentAnimationTexture_ = 0;
			}

			SetTexture(textures_.at(currentAnimationTexture_++));
			animationTimeOffset_ = clockRef_.getElapsedTime().asSeconds();
		}
	}
}
