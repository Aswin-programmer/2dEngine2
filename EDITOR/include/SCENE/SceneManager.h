#pragma once

#define SCENE_MANAGER() SceneManager::GetInstance()

class SceneObject;
class ToolManager;

class SceneManager
{
private:
	std::map<std::string, std::shared_ptr<SceneObject>> m_mapScenes;
	std::string m_sCurrentScene{ "" }, m_sCurrentTileSet{ "" };

	std::unique_ptr<ToolManager> m_pToolManager{ nullptr };

private:
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager( const SceneManager& ) = delete;			 // Deletes the copying of the class
	SceneManager& operator=( const SceneManager& ) = delete; // Deletes the assignment of the class.
public:
	static SceneManager& GetInstance();

	bool AddScene( const std::string& sSceneName );
	std::shared_ptr<SceneObject> GetScene( const std::string& sSceneName );
	std::shared_ptr<SceneObject> GetCurrentScene();
	std::vector<std::string> GetSceneNames() const;
	ToolManager& GetToolManager();
	void SetTileSet( const std::string& sTileset );

	inline void SetCurrentScene( const std::string& sSceneName ) { m_sCurrentScene = sSceneName; }
	inline const std::string& GetCurrentSceneName() const { return m_sCurrentScene; }
	inline const std::string& GetCurrentTilesetName() const { return m_sCurrentTileSet; }
};
