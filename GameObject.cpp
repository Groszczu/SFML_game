#include "GameObject.hpp"

namespace rstar
{
	GameObject::GameObject(GameDataPtr data) : data_( std::move(data) )	{}

	sf::Vector2f GameObject::GetPosition() const
	{
		return sprite_.getPosition();
	}

	void GameObject::SetTexture(sf::Texture const& texture)
	{
		sprite_.setTexture(texture);
	}

	void GameObjects::DrawAllObjects() const
	{
		for (auto const& object : objects_)
		{
			object->Draw();
		}
	}

	void GameObjects::AddObject(GameObjPtr &obj)
	{
		objects_.push_back(std::move(obj));
	}
}
