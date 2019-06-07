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
		// function loading texture to the texture map and setting its 'key'
		void LoadTexture(std::string const& textureName, std::vector<std::string> const& filePaths);

		// function giving access to the specific texture from the texture map
		// [if 'key' is correct]
		const sf::Texture& GetTexture(std::string const& textureName, unsigned textureNumber = 0) const;
		const std::vector<sf::Texture>& GetTexturesArray(std::string const& texturesName);

		// function loading font to the font map and setting its 'key'
		void LoadFont(std::string const& fontName, std::string const& filePath);

		// function giving access to the specific font from the font map
		// [if 'key' is correct]
		const sf::Font& GetFont(std::string const& fontName) const;

	private:
		std::map<std::string, std::vector<sf::Texture>> textures_;
		std::map<std::string, sf::Font> fonts_;
	};
}
