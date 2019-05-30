#include "Game.hpp"
#include "SplashState.hpp"

namespace rstar
{
	Game::Game(int width, int height, std::string const &title)
	{
		data_->window.create(sf::VideoMode( width, height ), title, sf::Style::Close | sf::Style::Titlebar);
		data_->stateMachine.SetState(std::make_unique<SplashState>(this->data_));
		run();
	}

	void Game::run() const
	{
		data_->window.setFramerateLimit(60);
		while (data_->window.isOpen())
		{
			data_->stateMachine.UpdatingStatesStack();

			data_->stateMachine.GetActiveState()->HandleInput();
			data_->stateMachine.GetActiveState()->Update();
			data_->stateMachine.GetActiveState()->Draw();
		}
	}
}
