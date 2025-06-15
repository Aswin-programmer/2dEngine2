#include "SCENE/SceneManager.h"

#include "SCENE/SceneObject.h"
#include "TOOLS/ToolManager.h"
#include "TOOLS/TileTool.h"
#include "UTILITIES/2dEngineUtilities.h"

#include "LOGGER/log.h"

SceneManager& SceneManager::GetInstance()
{
	static SceneManager instance{}; // Since it is static it will be created only one time and same instance will be returned everytime.
	return instance;
}

bool SceneManager::AddScene( const std::string& sSceneName )
{
	if ( m_mapScenes.contains( sSceneName ) )
	{
		LOG_ERROR( "Failed to add new scene object - [{}] already exists", sSceneName );
		return false;
	}

	auto result = m_mapScenes.emplace( sSceneName, std::move( std::make_shared<SceneObject>( sSceneName ) ) );
	return result.second;
}

std::shared_ptr<SceneObject> SceneManager::GetScene( const std::string& sSceneName )
{
	auto sceneItr = m_mapScenes.find( sSceneName );
	if ( sceneItr == m_mapScenes.end() )
	{
		LOG_ERROR( "Failed to get the scene object - [{}] does not exit", sSceneName );
		return nullptr;
	}

	return sceneItr->second;
}

std::shared_ptr<SceneObject> SceneManager::GetCurrentScene()
{
	if ( m_sCurrentScene.empty() )
	{
		return nullptr;
	}
	auto sceneItr = m_mapScenes.find( m_sCurrentScene );
	if ( sceneItr == m_mapScenes.end() )
	{
		LOG_ERROR( "Failed to get the scene object - [{}] does not exit", m_sCurrentScene );
		return nullptr;
	}

	return sceneItr->second;
}

std::vector<std::string> SceneManager::GetSceneNames() const
{
	return GetKeys( m_mapScenes );
}

ToolManager& SceneManager::GetToolManager()
{
	if ( !m_pToolManager )
	{
		m_pToolManager = std::make_unique<ToolManager>();
	}
	assert( m_pToolManager && "Tool manager must be valid" );

	return *m_pToolManager;
}

void SceneManager::SetTileSet( const std::string& sTileset )
{
	m_sCurrentTileSet = sTileset;
	if ( !m_pToolManager )
	{
		return;
	}

	auto pActiveTool = m_pToolManager->GetActiveTool();
	if ( pActiveTool )
	{
		pActiveTool->LoadSpriteTextureData( m_sCurrentTileSet );
	}
}
