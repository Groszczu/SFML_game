#include "Game.hpp"
#include "SplashState.hpp"
#include "DEFINITIONS.hpp"

namespace rstar
{
	Game::Game(int width, int height, std::string const &title)
	{
		data_->window.create(sf::VideoMode( width, height ), title, sf::Style::Close | sf::Style::Titlebar);

		sf::Image iconImage{};
		iconImage.loadFromFile(BOSS_FILEPATH);
		data_->window.setIcon(iconImage.getSize().x, iconImage.getSize().y, iconImage.getPixelsPtr());

		data_->stateMachine.SetState(std::make_unique<SplashState>(this->data_));
		run();
	}

	void Game::run() const
	{
		data_->window.setFramerateLimit(60);
		while (data_->window.isOpen())
		{
			data_->stateMachine.UpdateStatesStack();

			data_->stateMachine.GetActiveState()->HandleInput();
			data_->stateMachine.GetActiveState()->Update();
			data_->stateMachine.GetActiveState()->Draw();
		}
	}
}
