#pragma once

#include "WINDOW/INPUT/MouseButtons.h"

class Camera2D;
class Canvas;
class Registry;

class AbstractTool
{
private:
	glm::vec2 m_MouseScreenCoords, m_MouseWorldCoords;
	glm::vec2 m_GUICursorCoords, m_GUIRelativeCoords;
	glm::vec2 m_WindowPos, m_WindowSize;

	bool m_bAtivated, m_bOutOfBounds, m_bOverTileMapWindow;

private:
	void UpdateMouseWorldCoords();
	void CheckOutOfBounds( const Canvas& canvas );

protected:
	enum class EMouseButton
	{
		LEFT = MOUSE_LEFT,
		MIDDLE = MOUSE_MIDDLE,
		RIGHT = MOUSE_RIGHT,
		LAST = MOUSE_UNKNOWN
	};

	Registry* m_pRegistry{ nullptr };
	Camera2D* m_pCamera{ nullptr };

protected:
	bool MouseBtnJustPressed( EMouseButton eButton );
	bool MouseBtnJustReleased( EMouseButton eButton );
	bool MouseBtnPressed( EMouseButton eButton );
	bool MouseMoving();

	/*
	 * @brief used for different tools to make adjustments to the mouse position if  necessary.
	 */
	virtual void ExamineMousePosition() = 0;

	inline void SetMouseWorldCoords( const glm::vec2& newCoords ) { m_MouseWorldCoords = newCoords; }

public:
	AbstractTool();
	virtual ~AbstractTool() = default;

	/*
	 * @brief Update the tool. If overrided, ensure to call the parent update.
	 * This will make sure that the mouse world coords are updated accordingly
	 */
	virtual void Update( Canvas& canvas );

	/*
	 * @brief sets the current registry to the registry of hte current scene and the camera.
	 */
	bool SetupTool( Registry* pRegistry, Camera2D* pCamera );

	inline void SetRelativeCoords( const glm::vec2& relativeCoords ) { m_GUIRelativeCoords = relativeCoords; }
	inline void SetCursorCoords( const glm::vec2& cursorCoords ) { m_GUICursorCoords = cursorCoords; }
	inline void SetWindowPos( const glm::vec2& windowPos ) { m_WindowPos = windowPos; }
	inline void SetWindowSize( const glm::vec2& windowSize ) { m_WindowSize = windowSize; }

	inline const glm::vec2& GetMouseScreenCoords() const { return m_MouseScreenCoords; }
	inline const glm::vec2& GetMouseWorldCoords() const { return m_MouseWorldCoords; }

	inline void Activate() { m_bAtivated = true; }
	inline void DeActivate() { m_bAtivated = false; }
	inline void SetOverTilemapWindow( bool bOverWindow ) { m_bOverTileMapWindow = bOverWindow; }

	inline const bool IsActivated() const { return m_bAtivated; }
	inline const bool OutOfBounds() const { return m_bOutOfBounds; }
	inline const bool IsOverTilemapWindow() const { return m_bOverTileMapWindow; }
};
