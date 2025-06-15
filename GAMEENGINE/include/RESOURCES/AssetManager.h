#pragma once

enum class AssetType;

class Font;
class Texture;
class Shader;
class Registry;

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
	bool AddTexture( const std::string& textureName, const std::string& texturePath );
	/*
	 * @brief Checks to see if the texture exists based on the name and returns the texture.
	 * @param An std::string for the texture name to lookup.
	 * @return Returns the desired texture if it exists, else returns an empty texture object
	 */
	const Texture& GetTexture( const std::string& textureName );

	/*
	 * @brief Checks to see if the font exists, and if not, creates and loads the font into the
	 * asset manager.
	 * @param An std::string for the font name to be use as the key.
	 * @param An std::string for the texture file path to be loaded.
	 * @param A float for the font size
	 * @return Returns true if the font was created and loaded successfully, false otherwise.
	 */
	bool AddFont( const std::string& fontName, const std::string& fontPath, float fontSize = 32.f );

	/*
	 * @brief Checks to see if the font exists, and if not, creates and loads the font into the
	 * asset manager.
	 * @param An std::string for the font name to be use as the key.
	 * @param An unsigned char* of the font data
	 * @param A float for the font size
	 * @return Returns true if the font was created and loaded successfully, false otherwise.
	 */
	bool AddFontFromMemory( const std::string& fontName, unsigned char* fontData, float fontSize = 32.f );

	/*
	 * @brief Checks to see if the font exists based on the name and returns a std::shared_ptr<Font>.
	 * @param An std::string for the font name to lookup.
	 * @return Returns the desired font if it exists, else returns nullptr
	 */
	std::shared_ptr<Font> GetFont( const std::string& fontName );

	/*
	 * @brief Checks to see if the Shader exists, and if not, creates and loads the Shader into the
	 * asset manager.
	 * @param An std::string for the shader name to be use as the key.
	 * @param An std::string for the vertex shader file path to be loaded.
	 * @param An std::string for the fragment shader file path to be loaded.
	 * @return Returns true if the shader was created and loaded successfully, false otherwise.
	 */

	bool AddTextureFromMemory( const std::string& textureName, const unsigned char* imageData, int length );

	bool AddTilesetTexture( const std::string& textureName, const std::string& texturePath );

	bool AddEditorTextureFromMemory( const std::string& textureName, const unsigned char* imageData, int length );

	bool AddShader( const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath );
	/*
	 * @brief Checks to see if the shader exists based on the name and returns the Shader.
	 * @param An std::string for the shader name to lookup.
	 * @return Returns the desired shader if it exists, else returns an empty Shader object
	 */
	Shader& GetShader( const std::string& shaderName );

	std::vector<std::string> GetTilesetNames() const;

	inline const std::map<std::string, std::shared_ptr<Texture>>& GetAllTextures() const { return m_mapTextures; }

	std::vector<std::string> GetAssetKeyNames( AssetType eAssetType ) const;

	bool ChangeAssetName( const std::string& sOldName, const std::string& sNewName, AssetType eAssetType );

	bool CheckHasAsset( const std::string& sNameCheck, AssetType eAssetType );

	bool DeleteAsset( const std::string& sAssetName, AssetType eAssetType );

	// OpenAl Sound Buffer Setup:
	bool AddSoundEffect( std::string name, const char* filename );
	bool RemoveSoundEffect( std::string name );
	ALuint GetSoundEffect( std::string name );

	static void CreateLuaAssetManager( sol::state& lua, Registry& registry );

private:
	std::map<std::string, std::shared_ptr<Texture>> m_mapTextures{};
	std::map<std::string, std::shared_ptr<Shader>> m_mapShader{};
	std::map<std::string, std::shared_ptr<Font>> m_mapFonts{};
	std::map<std::string, ALuint> p_SoundEffectBuffers{}; //=>Destructor has to be make to destroy all the buffers which has been allocated for the program
	std::map<std::string, ALuint> p_MusicEffectBuffers{};
};
