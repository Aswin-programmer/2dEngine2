#include "AssetManager.h"

bool AssetManager::AddTexture(const std::string& textureName, const std::string& texturePath)
{
	if (m_mapTextures.find(textureName) != m_mapTextures.end())
	{
		LOG_WARN("Failed to add texture [{0}] -- Already exists!", textureName);
		return false;
	}
    auto texture = std::move(TextureLoader::createTexture(texturePath.c_str()));
    if (!texture)
    {
        LOG_ERROR("Failed to load texture [{0}] at path [{1}]", textureName, texturePath);
        return false;
    }
    m_mapTextures.emplace(textureName, std::move(texture));
    return true;
}

const Texture& AssetManager::GetTexture(const std::string& textureName)
{
    auto texItr = m_mapTextures.find(textureName);
    if (texItr == m_mapTextures.end())
    {
        LOG_ERROR("Failed to get texture [{0}] -- Does not exist!", textureName);
        return Texture();
    }
    return *texItr->second;
}

bool AssetManager::AddFont(const std::string& fontName, const std::string& fontPath, float fontSize)
{
    if (m_mapFonts.contains(fontName))
    {
        LOG_ERROR("Failed to add font [{0}] -- Already Exists!", fontName);
        return false;
    }

    auto pFont = FontLoader::Create(fontPath, fontSize);

    if (!pFont)
    {
        LOG_ERROR("Failed to add font [{}] at path [{}] -- to the asset manager!", fontName, fontPath);
        return false;
    }

    m_mapFonts.emplace(fontName, std::move(pFont));

    return true;
}

bool AssetManager::AddFontFromMemory(const std::string& fontName, unsigned char* fontData, float fontSize)
{
    if (m_mapFonts.contains(fontName))
    {
        LOG_ERROR("Failed to add font [{0}] -- Already Exists!", fontName);
        return false;
    }

    auto pFont = FontLoader::CreateFromMemory(fontData, fontSize);

    if (!pFont)
    {
        LOG_ERROR("Failed to add font [{0}] from memory -- to the asset manager!", fontName);
        return false;
    }

    m_mapFonts.emplace(fontName, std::move(pFont));

    return true;
}

std::shared_ptr<Font> AssetManager::GetFont(const std::string& fontName)
{
    auto fontItr = m_mapFonts.find(fontName);
    if (fontItr == m_mapFonts.end())
    {
        LOG_ERROR("Failed to get font [{0}] -- Does not exist!", fontName);
        return nullptr;
    }

    return fontItr->second;
}

bool AssetManager::AddTextureFromMemory(const std::string& textureName, const unsigned char* imageData, int length)
{
    // Check to see if the Texture already exist
    if (m_mapTextures.contains(textureName))
    {
        LOG_ERROR("AssetManager: Texture [{}] -- Already exists!", textureName);
        return false;
    }

    auto texture = std::move(TextureLoader::createTextureFromMemory(imageData, length));
    // Load the texture
    if (!texture)
    {
        LOG_ERROR("Unable to load texture [{}] from memory!", textureName);
        return false;
    }

    // Insert the texture into the map
    m_mapTextures.emplace(textureName, std::move(texture));

    return true;
}

bool AssetManager::AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
    // Check to see if the shader already exists
    if (m_mapShader.find(shaderName) != m_mapShader.end())
    {
        LOG_ERROR("Failed to add shader [{0}] -- Already Exists!", shaderName);
        return false;
    }
    // Create and load the shader
    auto shader = std::move(ShaderLoader::createShader(vertexPath.c_str(), fragmentPath.c_str()));
    if (!shader)
    {
        LOG_ERROR("Failed to load Shader [{0}] at vert path [{1}] and frag path [{2}]", shaderName, vertexPath, fragmentPath);
        return false;
    }
    m_mapShader.emplace(shaderName, std::move(shader));
    return true;
}

Shader& AssetManager::GetShader(const std::string& shaderName)
{
    auto shaderItr = m_mapShader.find(shaderName);
    if (shaderItr == m_mapShader.end())
    {
        LOG_ERROR("Failed to get shader [{0}] -- Does not exist!", shaderName);
        Shader shader{};
        return shader;
    }
    return *shaderItr->second;

}

void AssetManager::CreateLuaAssetManager(sol::state& lua, Registry& registry)
{
    auto& mainRegistry = MAIN_REGISTRY();
    auto& asset_manager = mainRegistry.GetAssetManager();

    lua.new_usertype<AssetManager>(
        "AssetManager",
        sol::no_constructor,
        "add_texture", [&](const std::string& assetName, const std::string& filepath)
        {
            return asset_manager.AddTexture(assetName, filepath);
        },
        "add_font", [&](const std::string& fontName, const std::string& fontPath, float fontSize)
        {
            return asset_manager.AddFont(fontName, fontPath, fontSize);
        }
    );
}
