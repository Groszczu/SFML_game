#include "GameObject.hpp"

namespace rstar
{
	GameObject::GameObject(GameDataPtr data) : data_( std::move(data) )	{}

	void GameObject::SetTexture(sf::Texture const& texture)
	{
		sprite_.setTexture(texture);
	}
}
