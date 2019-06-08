#pragma once
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "StateMachine.hpp"
#include "AssetManager.hpp"
#include "InputManager.hpp"

namespace rstar
{
	// structure that holds all game data
	struct GameData
	{
		StateMachine stateMachine;
		sf::RenderWindow window;
		AssetManager assets;
		InputManager input;
	};

	using GameDataPtr = std::shared_ptr<GameData>;

	// class containing main game loop
	class Game
	{
	public:
		Game(int width, int height, std::string const& title);

	private:
		GameDataPtr data_ = std::make_shared<GameData>();

		void run() const;
	};
}
