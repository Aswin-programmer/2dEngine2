#include "TilemapDisplay.h"

void TilemapDisplay::RenderTilemap()
{
	auto& mainRegistry = MAIN_REGISTRY();
	auto& editorFramebuffers = mainRegistry.GetContext<std::shared_ptr<EditorFramebuffers>>();
	auto& renderer = mainRegistry.GetContext<std::shared_ptr<Renderer>>();

	/*auto& renderSystem = mainRegistry.GetContext<std::shared_ptr<RenderSystem>>();
	auto& renderUISystem = mainRegistry.GetContext<std::shared_ptr<RenderUISystem>>();
	auto& renderShapeSystem = mainRegistry.GetContext<std::shared_ptr<RenderShapeSystem>>();*/

	const auto& fb = editorFramebuffers->mapFramebuffers[FramebufferType::TILEMAP];

	fb->Bind();
	renderer->SetViewport(0, 0, fb->Width(), fb->Height());
	renderer->SetClearColor(0.f, 0.f, 0.f, 1.f);
	renderer->ClearBuffers(true, true, false);

	auto& gridSystem = mainRegistry.GetContext<std::shared_ptr<GridSystem>>();
	gridSystem->Update(*m_pTilemapCam);

	/*renderSystem->Update();
	renderShapeSystem->Update();
	renderUISystem->Update( m_Registry.GetRegistry() );*/

	fb->Unbind();
	fb->CheckResize();

	renderer->SetClearColor(0.f, 0.f, 0.f, 1.f);
	renderer->ClearBuffers(true, true, false);

}

TilemapDisplay::TilemapDisplay()
	: m_pTilemapCam{ std::make_unique<Camera2D>() }
{
}

void TilemapDisplay::Draw()
{
	if (!ImGui::Begin("Tilemap Editor"))
	{
		ImGui::End();
		return;
	}

	RenderTilemap();

	auto& mainRegistry = MAIN_REGISTRY();

	if (ImGui::BeginChild("##tilemap", ImVec2{ 0, 0 }, false, ImGuiWindowFlags_NoScrollWithMouse))
	{
		auto& editorFramebuffers = mainRegistry.GetContext<std::shared_ptr<EditorFramebuffers>>();
		const auto& fb = editorFramebuffers->mapFramebuffers[FramebufferType::TILEMAP];

		ImVec2 imageSize{ static_cast<float>(fb->Width()), static_cast<float>(fb->Height()) };
		ImVec2 windowSize{ ImGui::GetWindowSize() };

		float x = (windowSize.x - imageSize.x) * 0.5f;
		float y = (windowSize.y - imageSize.y) * 0.5f;

		ImGui::SetCursorPos(ImVec2{ x, y });

		ImGui::Image((ImTextureID)fb->GetTextureID(), imageSize, ImVec2{ 0.f, 1.f }, ImVec2{ 1.f, 0.f });

		ImGui::EndChild();
	}
	ImGui::End();
}

void TilemapDisplay::Update()
{
	m_pTilemapCam->Update();
}
