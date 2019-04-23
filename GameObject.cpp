#include "GameObject.hpp"
#include "DEFINITIONS.hpp"

namespace rstar
{
	GameObject::GameObject(GameDataPtr data) : data_( std::move(data) )	{}

	void GameObject::SetTexture(sf::Texture const& texture)
	{
		sprite_.setTexture(texture);
	}

	bool GameObject::IsOutOfScreen() const
	{
		return GetPosition().y < 0 - GetBounds().height || GetPosition().y > WINDOW_HEIGHT + sprite_.getGlobalBounds().height;
	}
}
