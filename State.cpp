#include "State.hpp"
#include "DEFINITIONS.hpp"

namespace rstar
{
	void State::Fading(float time, sf::RenderWindow &window)
	{
		sf::Clock clock;
		sf::Texture texture;
		std::vector<sf::Sprite> blacks;
		texture.loadFromFile(BLACK_SHADOW_FILEPATH);

		blacks.reserve(20);

		while (blacks.size() != 20)
		{
			if (clock.getElapsedTime().asSeconds() > time)
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

	void State::FadingAway(float time, sf::RenderWindow &window, sf::Sprite const& back) 
	{
		sf::Clock clock;
		sf::Texture texture;
		std::vector<sf::Sprite> blacks;
		texture.loadFromFile(BLACK_SHADOW_FILEPATH);

		blacks.reserve(20);
		for (auto i = 0; i < 20; i++)
		{
			blacks.emplace_back(sf::Sprite{ texture });
		}

		for (auto &sprite : blacks)
		{
			window.draw(sprite);
		}
		window.display();
		window.clear();
		window.draw(back);

		while (!blacks.empty())
		{
			if (clock.getElapsedTime().asSeconds() > time)
			{
				blacks.pop_back();
				window.clear();
				window.draw(back);
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