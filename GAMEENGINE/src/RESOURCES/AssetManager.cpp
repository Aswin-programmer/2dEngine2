#include "pch.h"

#include "RESOURCES/AssetManager.h"

#include "UTILITIES/2dEngineUtilities.h"

#include "ECS/MainRegistry.h"
#include "LOGGER/log.h"
#include "RENDERER/SHADER/ShaderLoader.h";
#include "RENDERER/TEXTURE/TextureLoader.h";
#include "RENDERER/ESSENTIALS/FontLoader.h"

bool AssetManager::AddTexture( const std::string& textureName, const std::string& texturePath )
{
	if ( m_mapTextures.find( textureName ) != m_mapTextures.end() )
	{
		LOG_WARN( "Failed to add texture [{0}] -- Already exists!", textureName );
		return false;
	}
	auto texture = std::move( TextureLoader::createTexture( texturePath.c_str() ) );
	if ( !texture )
	{
		LOG_ERROR( "Failed to load texture [{0}] at path [{1}]", textureName, texturePath );
		return false;
	}
	m_mapTextures.emplace( textureName, std::move( texture ) );
	return true;
}

const Texture& AssetManager::GetTexture( const std::string& textureName )
{
	auto texItr = m_mapTextures.find( textureName );
	if ( texItr == m_mapTextures.end() )
	{
		LOG_ERROR( "Failed to get texture [{0}] -- Does not exist!", textureName );
		return Texture();
	}
	return *texItr->second;
}

bool AssetManager::AddFont( const std::string& fontName, const std::string& fontPath, float fontSize )
{
	if ( m_mapFonts.contains( fontName ) )
	{
		LOG_ERROR( "Failed to add font [{0}] -- Already Exists!", fontName );
		return false;
	}

	auto pFont = FontLoader::Create( fontPath, fontSize );

	if ( !pFont )
	{
		LOG_ERROR( "Failed to add font [{}] at path [{}] -- to the asset manager!", fontName, fontPath );
		return false;
	}

	m_mapFonts.emplace( fontName, std::move( pFont ) );

	return true;
}

bool AssetManager::AddFontFromMemory( const std::string& fontName, unsigned char* fontData, float fontSize )
{
	if ( m_mapFonts.contains( fontName ) )
	{
		LOG_ERROR( "Failed to add font [{0}] -- Already Exists!", fontName );
		return false;
	}

	auto pFont = FontLoader::CreateFromMemory( fontData, fontSize );

	if ( !pFont )
	{
		LOG_ERROR( "Failed to add font [{0}] from memory -- to the asset manager!", fontName );
		return false;
	}

	m_mapFonts.emplace( fontName, std::move( pFont ) );

	return true;
}

std::shared_ptr<Font> AssetManager::GetFont( const std::string& fontName )
{
	auto fontItr = m_mapFonts.find( fontName );
	if ( fontItr == m_mapFonts.end() )
	{
		LOG_ERROR( "Failed to get font [{0}] -- Does not exist!", fontName );
		return nullptr;
	}

	return fontItr->second;
}

bool AssetManager::AddTextureFromMemory( const std::string& textureName, const unsigned char* imageData, int length )
{
	// Check to see if the Texture already exist
	if ( m_mapTextures.contains( textureName ) )
	{
		LOG_ERROR( "AssetManager: Texture [{}] -- Already exists!", textureName );
		return false;
	}

	auto texture = std::move( TextureLoader::createTextureFromMemory( imageData, length ) );
	// Load the texture
	if ( !texture )
	{
		LOG_ERROR( "Unable to load texture [{}] from memory!", textureName );
		return false;
	}

	// Insert the texture into the map
	m_mapTextures.emplace( textureName, std::move( texture ) );

	return true;
}

bool AssetManager::AddTilesetTexture( const std::string& textureName, const std::string& texturePath )
{
	if ( m_mapTextures.find( textureName ) != m_mapTextures.end() )
	{
		LOG_WARN( "Failed to add texture [{0}] -- Already exists!", textureName );
		return false;
	}
	auto texture = std::move( TextureLoader::createTilesetTextureTexture( texturePath.c_str() ) );
	if ( !texture )
	{
		LOG_ERROR( "Failed to load texture [{0}] at path [{1}]", textureName, texturePath );
		return false;
	}
	m_mapTextures.emplace( textureName, std::move( texture ) );
	return true;
}

bool AssetManager::AddEditorTextureFromMemory( const std::string& textureName, const unsigned char* imageData, int length )
{
	// Check to see if the Texture already exist
	if ( m_mapTextures.contains( textureName ) )
	{
		LOG_ERROR( "AssetManager: Texture [{}] -- Already exists!", textureName );
		return false;
	}

	auto texture = std::move( TextureLoader::createEditorTextureFromMemory( imageData, length ) );
	// Load the texture
	if ( !texture )
	{
		LOG_ERROR( "Unable to load texture [{}] from memory!", textureName );
		return false;
	}

	// Insert the texture into the map
	m_mapTextures.emplace( textureName, std::move( texture ) );

	return true;
}

bool AssetManager::AddShader( const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath )
{
	// Check to see if the shader already exists
	if ( m_mapShader.find( shaderName ) != m_mapShader.end() )
	{
		LOG_ERROR( "Failed to add shader [{0}] -- Already Exists!", shaderName );
		return false;
	}
	// Create and load the shader
	auto shader = std::move( ShaderLoader::createShader( vertexPath.c_str(), fragmentPath.c_str() ) );
	if ( !shader )
	{
		LOG_ERROR( "Failed to load Shader [{0}] at vert path [{1}] and frag path [{2}]", shaderName, vertexPath, fragmentPath );
		return false;
	}
	m_mapShader.emplace( shaderName, std::move( shader ) );
	return true;
}

Shader& AssetManager::GetShader( const std::string& shaderName )
{
	auto shaderItr = m_mapShader.find( shaderName );
	if ( shaderItr == m_mapShader.end() )
	{
		LOG_ERROR( "Failed to get shader [{0}] -- Does not exist!", shaderName );
		Shader shader{};
		return shader;
	}
	return *shaderItr->second;
}

std::vector<std::string> AssetManager::GetTilesetNames() const
{
	return GetKeys( m_mapTextures, []( const auto& pair ) { return pair.second->IsTileset(); } );
}

std::vector<std::string> AssetManager::GetAssetKeyNames( AssetType eAssetType ) const
{
	switch ( eAssetType )
	{
		case AssetType::TEXTURE:
			return GetKeys( m_mapTextures, []( const auto& pair ) { return !pair.second->IsEditorTexture(); } );
		case AssetType::FONT:
			return GetKeys( m_mapFonts );
		case AssetType::SOUNDFX:
			return GetKeys( p_SoundEffectBuffers );
		case AssetType::MUSIC:
			return GetKeys( p_MusicEffectBuffers );
		default:
			assert( false && "Cannot get this type!" );
	}

	return std::vector<std::string>{};
}

bool AssetManager::ChangeAssetName( const std::string& sOldName, const std::string& sNewName, AssetType eAssetType )
{
	switch ( eAssetType )
	{
		case AssetType::TEXTURE:
			return KeyChange( m_mapTextures, sOldName, sNewName );
		case AssetType::FONT:
			return KeyChange( m_mapFonts, sOldName, sNewName );
		case AssetType::SOUNDFX:
			return KeyChange( p_SoundEffectBuffers, sOldName, sNewName );
		case AssetType::MUSIC:
			return KeyChange( p_MusicEffectBuffers, sOldName, sNewName );
		default:
			assert( false && "Cannot get this type!" );
	}

	return false;
}

bool AssetManager::CheckHasAsset( const std::string& sNameCheck, AssetType eAssetType )
{
	switch ( eAssetType )
	{
		case AssetType::TEXTURE:
			return m_mapTextures.contains( sNameCheck );
		case AssetType::FONT:
			return m_mapFonts.contains( sNameCheck );
		case AssetType::SOUNDFX:
			return p_SoundEffectBuffers.contains( sNameCheck );
		case AssetType::MUSIC:
			return p_MusicEffectBuffers.contains( sNameCheck );
		default:
			assert( false && "Cannot get this type!" );
	}

	return false;
}

bool AssetManager::DeleteAsset( const std::string& sAssetName, AssetType eAssetType )
{
	switch ( eAssetType )
	{
		case AssetType::TEXTURE:
			return std::erase_if( m_mapTextures, [&]( const auto& pair ) { // std::erase_if will return the number of elements deleted.
				return pair.first == sAssetName;
			} ) > 0;
		case AssetType::FONT:
			return std::erase_if( m_mapFonts, [&]( const auto& pair ) { // std::erase_if will return the number of elements deleted.
				return pair.first == sAssetName;
			} ) > 0;
		case AssetType::SOUNDFX:
			return std::erase_if( p_SoundEffectBuffers, [&]( const auto& pair ) { // std::erase_if will return the number of elements deleted.
				return pair.first == sAssetName;
			} ) > 0;
		case AssetType::MUSIC:
			return std::erase_if( p_MusicEffectBuffers, [&]( const auto& pair ) { // std::erase_if will return the number of elements deleted.
				return pair.first == sAssetName;
			} ) > 0;
		default:
			assert( false && "Cannot get this type!" );
	}
	return false;
}

bool AssetManager::AddSoundEffect( std::string name, const char* filename )
{
	// Check to see if the sound effect already exists
	if ( m_mapShader.find( name ) != m_mapShader.end() )
	{
		LOG_ERROR( "Failed to add sound effect [{0}] -- Already Exists!", name );
		return false;
	}
	ALenum err, format;
	ALuint buffer;
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	short* membuf;
	sf_count_t num_frames;
	ALsizei num_bytes;

	std::string fullPath = ( std::filesystem::current_path() / filename ).string();
	const char* path = fullPath.c_str();

	// Open the audio file and check that it's usable.
	sndfile = sf_open( path, SFM_READ, &sfinfo );
	if ( !sndfile )
	{
		fprintf( stderr, "Could not open audio in %s: %s\n", filename, sf_strerror( sndfile ) );
		return false;
	}

	if ( sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)( INT_MAX / sizeof( short ) ) / sfinfo.channels )
	{
		fprintf( stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames );
		sf_close( sndfile );
		return false;
	}

	/* Get the sound format, and figure out the OpenAL format */
	format = AL_NONE;
	if ( sfinfo.channels == 1 )
		format = AL_FORMAT_MONO16;
	else if ( sfinfo.channels == 2 )
		format = AL_FORMAT_STEREO16;
	else if ( sfinfo.channels == 3 )
	{
		if ( sf_command( sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0 ) == SF_AMBISONIC_B_FORMAT )
			format = AL_FORMAT_BFORMAT2D_16;
	}
	else if ( sfinfo.channels == 4 )
	{
		if ( sf_command( sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0 ) == SF_AMBISONIC_B_FORMAT )
			format = AL_FORMAT_BFORMAT3D_16;
	}
	if ( !format )
	{
		fprintf( stderr, "Unsupported channel count: %d\n", sfinfo.channels );
		sf_close( sndfile );
		return false;
	}

	/* Decode the whole audio file to a buffer. */
	membuf = static_cast<short*>( malloc( (size_t)( sfinfo.frames * sfinfo.channels ) * sizeof( short ) ) );

	num_frames = sf_readf_short( sndfile, membuf, sfinfo.frames );
	if ( num_frames < 1 )
	{
		free( membuf );
		sf_close( sndfile );
		fprintf( stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames );
		return false;
	}
	num_bytes = (ALsizei)( num_frames * sfinfo.channels ) * (ALsizei)sizeof( short );

	/* Buffer the audio data into a new buffer object, then free the data and
	 * close the file.
	 */
	buffer = 0;
	alGenBuffers( 1, &buffer );
	alBufferData( buffer, format, membuf, num_bytes, sfinfo.samplerate );

	free( membuf );
	sf_close( sndfile );

	/* Check if an error occured, and clean up if so. */
	err = alGetError();
	if ( err != AL_NO_ERROR )
	{
		fprintf( stderr, "OpenAL Error: %s\n", alGetString( err ) );
		if ( buffer && alIsBuffer( buffer ) )
			alDeleteBuffers( 1, &buffer );
		return false;
	}
	p_SoundEffectBuffers.emplace( name, buffer );
	return true;
}

bool AssetManager::RemoveSoundEffect( std::string name )
{
	auto it = p_SoundEffectBuffers.find( name );
	if ( it != p_SoundEffectBuffers.end() )
	{
		alDeleteBuffers( 1, &it->second ); // Delete OpenAL buffer
		p_SoundEffectBuffers.erase( it );  // Remove from map
		return true;
	}
	return false; // Couldn't find buffer with that name
}

ALuint AssetManager::GetSoundEffect( std::string name )
{
	auto it = p_SoundEffectBuffers.find( name );
	if ( it != p_SoundEffectBuffers.end() )
	{
		return it->second; // Return the OpenAL buffer handle
	}
	LUA_ERROR( "Failed to get the sound effect buffer!" );
	return 0; // 0 = invalid buffer
}

void AssetManager::CreateLuaAssetManager( sol::state& lua, Registry& registry )
{
	auto& mainRegistry = MAIN_REGISTRY();
	auto& asset_manager = mainRegistry.GetAssetManager();

	lua.new_usertype<AssetManager>(
		"AssetManager",
		sol::no_constructor,
		"add_texture", [&]( const std::string& assetName, const std::string& filepath ) { return asset_manager.AddTexture( assetName, filepath ); },
		"add_tileset_texture", [&]( const std::string& assetName, const std::string& filepath ) { return asset_manager.AddTilesetTexture( assetName, filepath ); },
		"add_font", [&]( const std::string& fontName, const std::string& fontPath, float fontSize ) { return asset_manager.AddFont( fontName, fontPath, fontSize ); } );
}
