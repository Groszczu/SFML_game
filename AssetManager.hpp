#pragma once
#include <map>
#include <SFML/Graphics.hpp>

namespace rstar
{
	// class that holds all textures and fonts using std::map for each
	// can set them or get access to them by public member functions
	class AssetManager
	{
	public:
		AssetManager() = default;
		~AssetManager() = default;

		// function loading texture to the texture map and setting its 'key'
		void LoadTexture(std::string const& textureName, std::string const& filePath);

		// function giving access to the specific texture from the texture map
		// [if 'key' is correct]
		sf::Texture& GetTexture(std::string const& textureName);

		// function loading font to the font map and setting its 'key'
		void LoadFont(std::string const& fontName, std::string const& filePath);

		// function giving access to the specific font from the font map
		// [if 'key' is correct]
		sf::Font& GetFont(std::string const& fontName);

	private:
		std::map<std::string, sf::Texture> textures_;
		std::map<std::string, sf::Font> fonts_;
	};
}
