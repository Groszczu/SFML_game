#include "AssetManager.hpp"

namespace rstar
{
	void AssetManager::LoadTexture(std::string const& textureName, std::string const& filePath)
	{
		sf::Texture tex;
		if (tex.loadFromFile(filePath))
		{
			textures_.emplace(textureName, tex);
		}
	}

	sf::Texture& AssetManager::GetTexture(std::string const& textureName)
	{
		return textures_.at(textureName);
	}

	void AssetManager::LoadFont(std::string const& fontName, std::string const& filePath)
	{
		sf::Font font;
		if (font.loadFromFile(filePath))
		{
			fonts_.emplace(fontName, font);
		}
	}

	sf::Font& AssetManager::GetFont(std::string const& fontName)
	{
		return fonts_.at(fontName);
	}
}