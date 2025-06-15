#pragma once

class Registry;

class Application
{
public:
	Application();
	~Application();
	std::unique_ptr<Registry> pRegistry;
	bool m_bIsRunning;

	static Application& GetInstance();
	void Run();

private:
	bool Initialize();
	bool LoadShaders();
	bool LoadEditorTextures();

	void ProcessEvents();
	void Update();
	void Render();
	void CleanUp();

	// TEMP IMGUI TESTING --> TODO: REMOVE TO OWN CLASS
	bool CreateDisplays();
	bool InitImGui();
	void Begin();
	void End();
	void RenderImGui();
};
