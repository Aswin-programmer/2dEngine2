#pragma once

class Window
{
public:
	// Initialize the window and OpenGL context
	static bool init( const char* title );

	// Process input
	static void processInput();

	// Check if the window should close
	static bool shouldClose();

	// Swap buffers and poll events
	static void update();

	// Clear the screen
	static float r, g, b, a;
	static void clearScreen();

	// Get the GLFW window pointer
	static GLFWwindow* getGLFWWindow();

	static float getdt() { return dt; }
	static void getFPS();
	static int getWidth() { return m_width; }
	static int getHeight() { return m_height; }

	static void cleanup();

private:
	static unsigned int m_width;
	static unsigned int m_height;
	static const char* m_title;
	static GLFWwindow* m_window;

	static float beginTime;
	static float endTime;
	static float dt;

	// Callback for window resizing
	static void framebufferSizeCallback( GLFWwindow* window, int width, int height );
};
