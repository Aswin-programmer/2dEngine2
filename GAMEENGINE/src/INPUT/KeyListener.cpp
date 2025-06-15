#include "pch.h"

#include "INPUT/KeyListener.h"

bool KeyListener::keyPressed[350] = { false };

void KeyListener::keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if ( action == GLFW_PRESS )
	{
		keyPressed[key] = true;
	}
	else if ( action == GLFW_RELEASE )
	{
		keyPressed[key] = false;
	}
}

bool KeyListener::isKeyPressed( int keyCode )
{
	return keyPressed[keyCode];
}
