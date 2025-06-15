#include "pch.h"

#include "INPUT/MouseListener.h"

double MouseListener::scrollX = 0.0;
double MouseListener::scrollY = 0.0;
double MouseListener::xPos = 0.0;
double MouseListener::yPos = 0.0;
double MouseListener::lastX = 0.0;
double MouseListener::lastY = 0.0;
bool MouseListener::isDragging = false;
bool MouseListener::mouseButtonPressed[3] = { false };
bool MouseListener::mouseButtonReleased[3] = { false };

void MouseListener::mousePosCallback( GLFWwindow* window, double xpos, double ypos )
{
	lastX = xPos;
	lastY = yPos;
	xPos = xpos;
	yPos = ypos;
	isDragging = mouseButtonPressed[0] || mouseButtonPressed[1] || mouseButtonPressed[2];
}

void MouseListener::mouseButtonCallback( GLFWwindow* window, int button, int action, int mods )
{
	if ( action == GLFW_PRESS )
	{
		if ( button < sizeof( mouseButtonPressed ) )
		{
			isDragging = true;
			mouseButtonPressed[button] = true;
			mouseButtonReleased[button] = false; // Reset release state on press
		}
	}
	else if ( action == GLFW_RELEASE )
	{
		if ( button < sizeof( mouseButtonPressed ) )
		{
			mouseButtonPressed[button] = false;
			mouseButtonReleased[button] = true; // Set release state
			isDragging = false;
		}
	}
}

void MouseListener::mouseScrollCallback( GLFWwindow* window, double xOffset, double yOffset )
{
	scrollX = xOffset;
	scrollY = yOffset;
}

void MouseListener::endFrame()
{
	scrollX = 0;
	scrollY = 0;
	lastX = xPos;
	lastY = yPos;
}

float MouseListener::getX()
{
	return xPos;
}

float MouseListener::getY()
{
	return yPos;
}

float MouseListener::getDx()
{
	return ( lastX - xPos );
}

float MouseListener::getDy()
{
	return ( lastY - yPos );
}

float MouseListener::getScrollX()
{
	return scrollX;
}

float MouseListener::getScrollY()
{
	return scrollY;
}

bool MouseListener::isDraging()
{
	return isDragging;
}

bool MouseListener::mouseButtonDown( int button )
{
	if ( button < sizeof( mouseButtonPressed ) )
	{
		return mouseButtonPressed[button];
	}
	else
	{
		return false;
	}
}

bool MouseListener::mouseButtonPressedThisFrame( int button )
{
	if ( button < sizeof( mouseButtonPressed ) )
	{
		return mouseButtonPressed[button];
	}
	return false;
}

bool MouseListener::mouseButtonReleasedThisFrame( int button )
{
	if ( button < sizeof( mouseButtonReleased ) )
	{
		return mouseButtonReleased[button];
	}
	return false;
}
