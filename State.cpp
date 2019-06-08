#include "State.hpp"
#include "DEFINITIONS.hpp"
#include <vector>

namespace rstar
{
	void State::Fading(float frameTime, sf::RenderWindow &window)
	{
		sf::Clock clock;
		sf::Texture texture;
		std::vector<sf::Sprite> blacks;
		texture.loadFromFile(BLACK_SHADOW_FILEPATH);

		blacks.reserve(FINAL_NUMBER_OF_SHADOWS);

		while (blacks.size() != FINAL_NUMBER_OF_SHADOWS)
		{
			if (clock.getElapsedTime().asSeconds() > frameTime)
			{
				blacks.emplace_back(sf::Sprite{ texture });

				for (auto &sprite : blacks)
				{
					window.draw(sprite);
				}
				window.display();
				clock.restart();
			}
		}
	}

	void State::FadingAway(float frameTime, sf::RenderWindow &window, sf::Sprite const& background) 
	{
		sf::Clock clock;
		sf::Texture texture;
		std::vector<sf::Sprite> blacks;
		texture.loadFromFile(BLACK_SHADOW_FILEPATH);

		blacks.reserve(FINAL_NUMBER_OF_SHADOWS);
		for (auto i = 0; i < FINAL_NUMBER_OF_SHADOWS; i++)
		{
			blacks.emplace_back(sf::Sprite{ texture });
		}

		for (auto &sprite : blacks)
		{
			window.draw(sprite);
		}
		window.display();
		window.clear();
		window.draw(background);

		while (!blacks.empty())
		{
			if (clock.getElapsedTime().asSeconds() > frameTime)
			{
				blacks.pop_back();
				window.clear();
				window.draw(background);
				for (auto &sprite : blacks)
				{
					window.draw(sprite);
				}
				window.display();
				clock.restart();
			}
		}
	}

}