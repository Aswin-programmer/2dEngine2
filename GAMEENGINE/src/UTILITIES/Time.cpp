#include "pch.h"

#include "UTILITIES/Time.h"

float Time::timeStarted = glfwGetTime();

float Time::getTime()
{
	return ( glfwGetTime() - timeStarted );
}
