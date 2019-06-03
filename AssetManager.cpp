#include "AssetManager.hpp"

namespace rstar
{
	void AssetManager::LoadTexture(std::string const& textureName, std::vector<std::string> const& filePaths)
	{
		for (auto const& path : filePaths)
		{
			sf::Texture tex;
			if (tex.loadFromFile(path))
			{
				textures_[textureName].emplace_back(tex);
			}
		}
	}

	const sf::Texture& AssetManager::GetTexture(std::string const& textureName, unsigned textureNumber) const
	{
		return textures_.at(textureName).at(textureNumber);
	}

	const std::vector<sf::Texture>& AssetManager::GetTexturesArray(std::string const& texturesName) const
	{
		return textures_.at(texturesName);
	}


	void AssetManager::LoadFont(std::string const& fontName, std::string const& filePath)
	{
		sf::Font font;
		if (font.loadFromFile(filePath))
		{
			fonts_.emplace(fontName, font);
		}
	}

	const sf::Font& AssetManager::GetFont(std::string const& fontName) const
	{
		return fonts_.at(fontName);
	}
}