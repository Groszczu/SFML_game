#pragma once
#include "Game.hpp"
#include <memory>
#include <vector>

namespace rstar
{
	// pure virtual generic GameObject class
	// contains sprite and virtual functions:
	// --void Update() = 0
	// --void Draw() = 0
	class GameObject
	{
	public:
		explicit GameObject(GameDataPtr data);
		virtual ~GameObject() = default;

		// returns width of GameObject's sprite
		float GetWidth() const { return sprite_.getGlobalBounds().width; }
		// returns height of GameObject's sprite
		float GetHeight() const { return sprite_.getGlobalBounds().height; }

		sf::Vector2f GetPosition() const;

		// sets new GameObject's sprite texture
		void SetTexture(sf::Texture const& texture);

		// pure virtual function
		virtual void Update() = 0;
		// pure virtual function
		virtual void Draw() const = 0;
	protected:
		sf::Sprite sprite_;
		GameDataPtr data_;
	};

	using GameObjPtr = std::unique_ptr<GameObject>;

	// class containing all GameObjects
	// [std::vector as container holding pointers to GameObjects]
	class GameObjects
	{
	public:
		GameObjects() = default;

		// calls Game::Object::Draw() for every element in collection
		void DrawAllObjects() const;

		// adds GameObject pointer to the collection
		void AddObject(GameObjPtr &obj);
	private:
		std::vector<GameObjPtr> objects_;
	};
}
