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
		return GetPosition().y < 0 - GetBounds().height || GetPosition().y > WINDOW_HEIGHT + GetBounds().height ||
			GetPosition().x < 0 - GetBounds().width || GetPosition().x > WINDOW_WIDTH + GetBounds().width;
	}
}
