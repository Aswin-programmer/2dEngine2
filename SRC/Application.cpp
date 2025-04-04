#include "Application.h"

Application& Application::GetInstance()
{
	static Application app{};
	return app;
}

Application::Application()
	:
	pRegistry{ nullptr }, m_bIsRunning{ true }
{
}

Application::~Application()
{
}

void Application::Run()
{
	if (!Initialize())
	{
		LOG_ERROR("Initialization Failed!");
		return;
	}
	while (m_bIsRunning && !Window::shouldClose())
	{
		Window::clearScreen();
		Window::processInput();

		ProcessEvents();
		Update();
		if (Window::getdt() > 0.0f)
		{
			Render();
		}
		Window::update();
		
	}
	CleanUp();
}

bool Application::Initialize()
{
	std::cout << "Hello World!" << std::endl;

	//Logger
	log::Init();

	Window::init(640, 480, "2DENGINE");

	auto assetManager = std::make_shared<AssetManager>();
	if (!assetManager)
	{
		LOG_ERROR("Failed to create the asset manager");
		return -1;
	}

	if (!assetManager->AddTexture("texture1", "ASSETS/IMAGES/spritesheet.png"))
	{
		LOG_ERROR("Failed to add texture!");
		return -1;
	}

	// Create a new entity -- for test
	pRegistry = std::make_unique<Registry>();
	if (!pRegistry)
	{
		LOG_ERROR("Failed to create a entt registry!");
	}

	auto renderer = std::make_shared<Renderer>();
	if (!pRegistry->AddToContext<std::shared_ptr<Renderer>>(renderer))
	{
		LOG_ERROR("Failed to add the renderer to the registry context");
		return false;
	}

	renderer->SetLineWidth(4.f);

	//Create the lua state
	auto lua = std::make_shared<sol::state>();
	if (!lua)
	{
		LOG_ERROR("Failed to create the lua state!");
		return false;
	}
	lua->open_libraries(
		sol::lib::base,
		sol::lib::math,
		sol::lib::os,
		sol::lib::table,
		sol::lib::io,
		sol::lib::string
	);
	if (!pRegistry->AddToContext<std::shared_ptr<sol::state>>(lua))
	{
		LOG_ERROR("Failed to add the sol::state to the registry context");
		return false;
	}

	auto scriptSystem = std::make_shared<ScriptingSystem>(*pRegistry);
	if (!scriptSystem)
	{
		LOG_ERROR("Failed to create the script system!");
		return false;
	}

	if (!pRegistry->AddToContext<std::shared_ptr<ScriptingSystem>>(scriptSystem))
	{
		LOG_ERROR("Failed to add the scriptSystem to the registry context");
		return false;
	}

	//Setting up the rendersystem:
	auto renderSystem = std::make_shared<RenderSystem>(*pRegistry);
	if (!renderSystem)
	{
		LOG_ERROR("Failed to create the render system!");
		return false;
	}
	if (!pRegistry->AddToContext<std::shared_ptr<RenderSystem>>(renderSystem))
	{
		LOG_ERROR("Failed to add the rendersystem to the registry context");
		return false;
	}

	//Setting up the animation system
	auto animationSystem = std::make_shared<AnimationSystem>(*pRegistry);
	if (!animationSystem)
	{
		LOG_ERROR("Failed to create the animation system!");
		return false;
	}
	if (!pRegistry->AddToContext<std::shared_ptr<AnimationSystem>>(animationSystem))
	{
		LOG_ERROR("Failed to add the animation system to the registry context!");
		return false;
	}

	// Create a temp camera
	auto camera = std::make_shared<Camera2D>();

	if (!pRegistry->AddToContext<std::shared_ptr<AssetManager>>(assetManager))
	{
		LOG_ERROR("Failed to add the asset manager to the registry context!");
		return false;
	}
	if (!pRegistry->AddToContext<std::shared_ptr<Camera2D>>(camera))
	{
		LOG_ERROR("Failed to add the camera to the registry context!");
		return false;
	}

	if (!LoadShaders())
	{
		LOG_ERROR("Failed to load the shaders!");
		return false;
	}

	//Initialize SoundSystems:
	auto soundSystem = std::make_shared<SoundSystem>();
	if (!pRegistry->AddToContext<std::shared_ptr<SoundSystem>>(soundSystem))
	{
		LOG_ERROR("Failed to add the camera to the registry context!");
		return false;
	}
	if (!soundSystem->Initialize())
	{
		LOG_ERROR("Failed to initialize the SoundSystem");
	}

	if (!soundSystem->LoadBanks("C:/Aswin_Game_DEV/2DEngine2/Project1/ASSETS/SPACESHOOTER/MUSIC/spaceshooter/Build/Desktop"))
	{
		LOG_ERROR("Failed to LoadBanks for SoundSystem");
	}

	//Lua and ENTT::meta BINDING
	ScriptingSystem::RegisterLuaBindings(*lua, *pRegistry);
	ScriptingSystem::RegisterLuaFunctions(*lua);

	if (!scriptSystem->LoadMainScript(*lua))
	{
		LOG_ERROR("Failed to load the main lua script");
	}


	// Temp Load pixel font
	if (!assetManager->AddFont("pixel", "ASSETS/FONTS/Aaargh.ttf"))
	{
		LOG_ERROR("Failed to load pixel font!");
		return false;
	}
	// ============================================================================
	// Test for Drawing Text --> TO BE DELETED!
	// ============================================================================
	auto pFont = assetManager->GetFont("pixel");
	renderer->DrawText2D(
		Text{
			.position = glm::vec2{ 225.f, 200.f },
			.textStr = "SCION2D",
			.pFont = pFont
		}
	);

	renderer->DrawText2D(
		Text{
		.position = glm::vec2{ 0.f, 300.f },
			.textStr = "TEXT BATCH RENDERING!",
			.pFont = pFont
		}
	);
	// ============================================================================


	return true;
}

bool Application::LoadShaders()
{
	auto& assetManager = pRegistry->GetContext<std::shared_ptr<AssetManager>>();

	if (!assetManager)
	{
		LOG_ERROR("Failed to get the asset manager from the registry context");
		return false;
	}
	if (!assetManager->AddShader("shader1", "ASSETS/SHADER/shader.vert"
		, "ASSETS/SHADER/shader.frag"))
	{
		LOG_ERROR("Failed to add shader!");
		return false;
	}

	if (!assetManager->AddShader("color", "ASSETS/SHADER/colorShader.vert",
		"ASSETS/SHADER/colorShader.frag"))
	{
		LOG_ERROR("Failed to add the color shader to the asset manager");
		return false;
	}

	if (!assetManager->AddShader("circle", "ASSETS/SHADER/circleShader.vert",
		"ASSETS/SHADER/circleShader.frag"))
	{
		LOG_ERROR("Failed to add the color shader to the asset manager");
		return false;
	}

	if (!assetManager->AddShader("font", "ASSETS/SHADER/fontShader.vert",
		"ASSETS/SHADER/fontShader.frag"))
	{
		LOG_ERROR("Failed to add the font shader to the asset manager");
		return false;
	}

	return true;
}

void Application::ProcessEvents()
{
	auto& inputManager = InputManager::GetInstance();
	auto& keyboard = inputManager.GetKeyboard();
	auto& mouse = inputManager.GetMouse();

	// Process Events
	glfwPollEvents();

	// Check if window should close
	if (glfwWindowShouldClose(Window::getGLFWWindow())) {
		m_bIsRunning = false;
	}

	// Check ESC key directly for quitting
	if (glfwGetKey(Window::getGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		m_bIsRunning = false;
	}

	// Instead of directly handling keyboard events here, you'll want to set up GLFW callbacks
	// Add this during window/input initialization:
	glfwSetKeyCallback(Window::getGLFWWindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto& inputManager = InputManager::GetInstance();
		auto& keyboard = inputManager.GetKeyboard();

		if (action == GLFW_PRESS) {
			keyboard.OnKeyPressed(key);
		}
		else if (action == GLFW_RELEASE) {
			keyboard.OnKeyReleased(key);
		}
		});

	// Set up mouse button callback
	glfwSetMouseButtonCallback(Window::getGLFWWindow(), [](GLFWwindow* window, int button, int action, int mods) {
		auto& inputManager = InputManager::GetInstance();
		auto& mouse = inputManager.GetMouse();

		if (action == GLFW_PRESS) {
			mouse.OnBtnPressed(button);
		}
		else if (action == GLFW_RELEASE) {
			mouse.OnBtnReleased(button);
		}
		});

	// Set up mouse scroll callback
	glfwSetScrollCallback(Window::getGLFWWindow(), [](GLFWwindow* window, double xoffset, double yoffset) {
		auto& inputManager = InputManager::GetInstance();
		auto& mouse = inputManager.GetMouse();

		mouse.SetMouseWheelX(static_cast<int>(xoffset));
		mouse.SetMouseWheelY(static_cast<int>(yoffset));
		});

	// Set up mouse movement callback
	glfwSetCursorPosCallback(Window::getGLFWWindow(), [](GLFWwindow* window, double xpos, double ypos) {
		auto& inputManager = InputManager::GetInstance();
		auto& mouse = inputManager.GetMouse();

		mouse.SetMouseMoving(true);
		});
}

void Application::Update()
{
	auto view = pRegistry->GetRegistry().view<TransformComponent, SpriteComponent>();

	auto& camera = pRegistry->GetContext<std::shared_ptr<Camera2D>>();
	if (!camera)
	{
		LOG_ERROR("Failed to get the camera from the registry context!");
		return;
	}

	camera->Update();

	auto& scriptSystem = pRegistry->GetContext<std::shared_ptr<ScriptingSystem>>();
	scriptSystem->Update();

	auto& animationSystem = pRegistry->GetContext<std::shared_ptr<AnimationSystem>>();
	animationSystem->Update();

	// Update inputs
	auto& inputManager = InputManager::GetInstance();
	auto& keyboard = inputManager.GetKeyboard();
	keyboard.Update();
	auto& mouse = inputManager.GetMouse();
	mouse.Update();

	//Update SoundSystem
	auto& soundSystem = pRegistry->GetContext<std::shared_ptr<SoundSystem>>();
	soundSystem->Update(Window::getdt(), *pRegistry);

}

void Application::Render()
{
	auto& renderSystem = pRegistry->GetContext<std::shared_ptr<RenderSystem>>();

	auto& camera = pRegistry->GetContext<std::shared_ptr<Camera2D>>();
	auto& renderer = pRegistry->GetContext<std::shared_ptr<Renderer>>();
	auto& assetManager = pRegistry->GetContext<std::shared_ptr<AssetManager>>();

	auto& shader = assetManager->GetShader("color");
	auto& circleShader = assetManager->GetShader("circle");
	auto& fontShader = assetManager->GetShader("font");

	auto& scriptSystem = pRegistry->GetContext<std::shared_ptr<ScriptingSystem>>();
	scriptSystem->Render();

	renderSystem->Update();

	renderer->DrawLines(shader, *camera);
	renderer->DrawFilledRects(shader, *camera);
	renderer->DrawCircles(circleShader, *camera);
	renderer->DrawAllText(fontShader, *camera);

	renderer->ClearPrimitives();
}

void Application::CleanUp()
{
	Window::cleanup();
}
