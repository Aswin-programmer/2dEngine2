#pragma once

class TileTool;
class Canvas;

enum class EToolType;

class ToolManager
{
private:
	std::map<EToolType, std::unique_ptr<TileTool>> m_mapTools;

public:
	ToolManager();
	~ToolManager() = default;

	void Update( Canvas& canvas );
	/*
	 * @brief Activates the tool based on the passed in tool type.
	 * This will also deactivate all other tools.
	 * @param The tool type we want to active.
	 */
	void SetToolActive( EToolType eToolType );

	/*
	 * @brief Get the current activated tool.
	 * @return Returns a pointer to a TileTool if activated, else returns nullptr.
	 */
	TileTool* GetActiveTool();
};
