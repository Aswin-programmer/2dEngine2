#pragma once

#include "IDisplay.h"

class Registry;

class SceneDisplay : public IDisplay
{
private:
	bool m_bPlayScene, m_bSceneLoaded;

private:
	void LoadScene();
	void UnloadScene();
	void RenderScene();

public:
	SceneDisplay( Registry& registry );
	virtual ~SceneDisplay() = default;

	virtual void Draw() override;
	virtual void Update() override;
};
