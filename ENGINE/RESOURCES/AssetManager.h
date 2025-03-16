#pragma once

#include "../RENDERER/TEXTURE/TextureLoader.h";
#include "../RENDERER/SHADER/ShaderLoader.h";
#include "../SOUNDS/ESSENTIALS/Music.h"
#include "../ECS/Registry.h"

#include "sol/sol.hpp"

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager() = default;
	/*
		* @brief Checks to see if the texture exists, and if not, creates and loads the texture into the
		* asset manager.
		* @param An std::string for the texture name to be use as the key.
		* @param An std::string for the texture file path to be loaded.
		* @param A bool value to determine if it is pixel art. That controls the type of Min/Mag filter to
		* use.
		* @return Returns true if the texture was created and loaded successfully, false otherwise.
		*/
	bool AddTexture(const std::string& textureName, const std::string& texturePath);
	/*
	* @brief Checks to see if the texture exists based on the name and returns the texture.
	* @param An std::string for the texture name to lookup.
	* @return Returns the desired texture if it exists, else returns an empty texture object
	*/
	const Texture& GetTexture(const std::string& textureName);

	/*
	* @brief Checks to see if the Shader exists, and if not, creates and loads the Shader into the
	* asset manager.
	* @param An std::string for the shader name to be use as the key.
	* @param An std::string for the vertex shader file path to be loaded.
	* @param An std::string for the fragment shader file path to be loaded.
	* @return Returns true if the shader was created and loaded successfully, false otherwise.
	*/
	bool AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	/*
	* @brief Checks to see if the shader exists based on the name and returns the Shader.
	* @param An std::string for the shader name to lookup.
	* @return Returns the desired shader if it exists, else returns an empty Shader object
	*/
	Shader& GetShader(const std::string& shaderName);

	bool AddMusic(const std::string& musicName, const std::string& filepath);
	std::shared_ptr<Music> GetMusic(const std::string& musicName);

	static void CreateLuaAssetManager(sol::state& lua, Registry& registry);

private:
	std::map<std::string, std::shared_ptr<Texture>> m_mapTextures{};
	std::map<std::string, std::shared_ptr<Shader>> m_mapShader{};
	std::map<std::string, std::shared_ptr<Music>> m_mapMusic{};
};

