#pragma once

class KeyListener
{
public:
	static void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods );
	static bool isKeyPressed( int keyCode );

private:
	static bool keyPressed[350];
};
