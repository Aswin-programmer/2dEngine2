#include "ToolManager.h"

ToolManager::ToolManager()
{
	auto pCreateTileTool = std::make_unique<CreateTileTool>();
	m_mapTools.emplace(EToolType::CREATE_TILE, std::move(pCreateTileTool));

	// TODO: Add other tools as needed.

	SetToolActive(EToolType::CREATE_TILE);
}

void ToolManager::Update(Canvas& canvas)
{
	auto activeTool = std::ranges::find_if(m_mapTools, [](auto& tool) {return tool.second->IsActivated(); });
	if (activeTool != m_mapTools.end())
	{
		activeTool->second->Update(canvas);
	}
}

void ToolManager::SetToolActive(EToolType eToolType)
{
	for (auto& [eType, pTool] : m_mapTools)
	{
		if (eType == eToolType)
		{
			pTool->Activate();
		}
		else
		{
			pTool->DeActivate();
		}
	}
}

TileTool* ToolManager::GetActiveTool()
{
	auto activeTool = std::ranges::find_if(m_mapTools, [](auto& tool) {return tool.second->IsActivated(); });
	if (activeTool != m_mapTools.end())
	{
		return activeTool->second.get();
	}
	return nullptr;
}
