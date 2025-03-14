#pragma once

#include <iostream>
#include <memory>

#include "../WINDOW/Window.h"
#include "../LOGGER/log.h"
#include "../RESOURCES/AssetManager.h"
#include "../RENDERER/CORE/Camera2D.h"
#include "../RENDERER/ESSENTIALS/Vertex.h"
#include "../ECS/Entity.h";
#include "../ECS/COMPONENTS/SpriteComponent.h";
#include "../ECS/COMPONENTS/TransformComponent.h";
#include "../ECS/COMPONENTS/Identification.h"
#include "../ECS/COMPONENTS/SoundEmitter.h"

#include "../SYSTEMS/ScriptingSystem.h"
#include "../SYSTEMS/RenderSystem.h"
#include "../SYSTEMS/AnimationSystem.h"
#include "../SYSTEMS/SoundSystem.h"

#include "../SCRIPTING/InputManager.h"
#include "../WINDOW/INPUT/Keyboard.h"
#include "../WINDOW/INPUT/Mouse.h"

#include "../SOUNDS/MUSICPLAYER/MusicPlayer.h"

#include <glm/glm.hpp>

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
	void ProcessEvents();
	void Update();
	void Render();
	void CleanUp();
};

