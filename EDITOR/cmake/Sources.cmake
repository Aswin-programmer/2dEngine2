# ==============================================================================
# Aswin2DEditor - C++ Library CMake Sources
# ==============================================================================

set(PUBLIC_HEADERS
	# --- Application ---
	${ASWIN_EDITOR_INCLUDE_DIR}/Application.h

	# --- DISPLAYS ---
	${ASWIN_EDITOR_INCLUDE_DIR}/DISPLAYS/AssetDisplay.h
	${ASWIN_EDITOR_INCLUDE_DIR}/DISPLAYS/IDisplay.h
	${ASWIN_EDITOR_INCLUDE_DIR}/DISPLAYS/LogDisplay.h
	${ASWIN_EDITOR_INCLUDE_DIR}/DISPLAYS/MenuDisplay.h
	${ASWIN_EDITOR_INCLUDE_DIR}/DISPLAYS/SceneDisplay.h
	${ASWIN_EDITOR_INCLUDE_DIR}/DISPLAYS/TilemapDisplay.h
	${ASWIN_EDITOR_INCLUDE_DIR}/DISPLAYS/TilesetDisplay.h

	# --- SCENE ---
	${ASWIN_EDITOR_INCLUDE_DIR}/SCENE/SceneManager.h
	${ASWIN_EDITOR_INCLUDE_DIR}/SCENE/SceneObject.h

	# --- SYSTEMS ---
	${ASWIN_EDITOR_INCLUDE_DIR}/SYSTEMS/GridSystem.h

	# --- TOOLS ---
	${ASWIN_EDITOR_INCLUDE_DIR}/TOOLS/AbstractTool.h
	${ASWIN_EDITOR_INCLUDE_DIR}/TOOLS/CreateTileTool.h
	${ASWIN_EDITOR_INCLUDE_DIR}/TOOLS/RectFillTool.h
	${ASWIN_EDITOR_INCLUDE_DIR}/TOOLS/TileTool.h
	${ASWIN_EDITOR_INCLUDE_DIR}/TOOLS/ToolAccessories.h
	${ASWIN_EDITOR_INCLUDE_DIR}/TOOLS/ToolManager.h

	# --- UTILITIES ---
	${ASWIN_EDITOR_INCLUDE_DIR}/UTILITIES/EditorFrameBuffers.h
	${ASWIN_EDITOR_INCLUDE_DIR}/UTILITIES/EditorTextures.h
	${ASWIN_EDITOR_INCLUDE_DIR}/UTILITIES/EditorUtilities.h


)

set(PRIVATE_SOURCES
	# --- Application ---
	${ASWIN_EDITOR_SOURCE_DIR}/main.cpp
	${ASWIN_EDITOR_SOURCE_DIR}/Application.cpp

	# --- DISPLAYS ---
	${ASWIN_EDITOR_SOURCE_DIR}/DISPLAYS/AssetDisplay.cpp
	${ASWIN_EDITOR_SOURCE_DIR}/DISPLAYS/LogDisplay.cpp
	${ASWIN_EDITOR_SOURCE_DIR}/DISPLAYS/MenuDisplay.cpp
	${ASWIN_EDITOR_SOURCE_DIR}/DISPLAYS/SceneDisplay.cpp
	${ASWIN_EDITOR_SOURCE_DIR}/DISPLAYS/TilemapDisplay.cpp
	${ASWIN_EDITOR_SOURCE_DIR}/DISPLAYS/TilesetDisplay.cpp

	# --- SCENE ---
	${ASWIN_EDITOR_SOURCE_DIR}/SCENE/SceneManager.cpp
	${ASWIN_EDITOR_SOURCE_DIR}/SCENE/SceneObject.cpp

	# --- SYSTEMS ---
	${ASWIN_EDITOR_SOURCE_DIR}/SYSTEMS/GridSystem.cpp

	# --- TOOLS ---
	${ASWIN_EDITOR_SOURCE_DIR}/TOOLS/AbstractTool.cpp
	${ASWIN_EDITOR_SOURCE_DIR}/TOOLS/CreateTileTool.cpp
	${ASWIN_EDITOR_SOURCE_DIR}/TOOLS/RectFillTool.cpp
	${ASWIN_EDITOR_SOURCE_DIR}/TOOLS/TileTool.cpp
	${ASWIN_EDITOR_SOURCE_DIR}/TOOLS/ToolManager.cpp



)
