#pragma once

#include "ECS/Registry.h"
#include "UTILITIES/EditorUtilities.h"

class SceneObject
{
private:
	Registry m_Registry, m_RuntimeRegistry;
	std::string m_sSceneName;
	Canvas m_Canvas;

public:
	SceneObject( const std::string& sceneName );
	~SceneObject() = default;

	void CopySceneToRuntime();
	void ClearRuntimeScene();

	inline Canvas& GetCanvas() { return m_Canvas; }
	inline const std::string& GetName() { return m_sSceneName; }
	inline Registry& GetRegistry() { return m_Registry; }
	inline Registry* GetRegistryPtr() { return &m_Registry; }
	inline Registry& GetRuntimeRegistry() { return m_RuntimeRegistry; }
};
