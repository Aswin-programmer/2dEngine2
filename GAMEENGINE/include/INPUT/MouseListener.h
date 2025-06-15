#pragma once

class MouseListener
{
public:
	static void mousePosCallback( GLFWwindow* window, double xpos, double ypos );
	static void mouseButtonCallback( GLFWwindow* window, int button, int action, int mods );
	static void mouseScrollCallback( GLFWwindow* window, double xOffset, double yOffset );
	static void endFrame();
	static float getX();
	static float getY();
	static float getDx();
	static float getDy();
	static float getScrollX();
	static float getScrollY();
	static bool isDraging();
	static bool mouseButtonDown( int button );
	static bool mouseButtonPressedThisFrame( int button );
	static bool mouseButtonReleasedThisFrame( int button );

private:
	static double scrollX, scrollY;
	static double xPos, yPos, lastX, lastY;
	static bool mouseButtonPressed[3];
	static bool mouseButtonReleased[3];
	static bool isDragging;
};
