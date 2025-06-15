#include "SCENE/SceneObject.h"

SceneObject::SceneObject( const std::string& sceneName ) : m_sSceneName( sceneName ),
														   m_Registry{},
														   m_RuntimeRegistry{},
														   m_Canvas{}
{
}

void SceneObject::CopySceneToRuntime()
{
}

void SceneObject::ClearRuntimeScene()
{
}
