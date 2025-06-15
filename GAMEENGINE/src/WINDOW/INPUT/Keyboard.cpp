#include "pch.h"

#include "WINDOW/INPUT/Keyboard.h"

#include "LOGGER/log.h"

#include "WINDOW/Window.h"
#include "WINDOW/INPUT/Button.h"
#include "WINDOW/INPUT/Keys.h"

Keyboard::Keyboard()
	: m_window( Window::getGLFWWindow() ),
	  m_mapButtons{
		  { KEY_BACKSPACE, Button{} }, { KEY_TAB, Button{} },
		  { KEY_RETURN, Button{} }, { KEY_PAUSE, Button{} },
		  { KEY_ESCAPE, Button{} }, { KEY_SPACE, Button{} },
		  { KEY_COMMA, Button{} }, { KEY_MINUS, Button{} },
		  { KEY_PERIOD, Button{} }, { KEY_SLASH, Button{} },
		  { KEY_0, Button{} }, { KEY_1, Button{} },
		  { KEY_2, Button{} }, { KEY_3, Button{} },
		  { KEY_4, Button{} }, { KEY_5, Button{} },
		  { KEY_6, Button{} }, { KEY_7, Button{} },
		  { KEY_8, Button{} }, { KEY_9, Button{} },
		  { KEY_SEMICOLON, Button{} }, { KEY_EQUALS, Button{} },
		  { KEY_LEFTBRACKET, Button{} }, { KEY_BACKSLASH, Button{} },
		  { KEY_RIGHTBRACKET, Button{} },
		  { KEY_A, Button{} }, { KEY_B, Button{} },
		  { KEY_C, Button{} }, { KEY_D, Button{} },
		  { KEY_E, Button{} }, { KEY_F, Button{} },
		  { KEY_G, Button{} }, { KEY_H, Button{} },
		  { KEY_I, Button{} }, { KEY_J, Button{} },
		  { KEY_K, Button{} }, { KEY_L, Button{} },
		  { KEY_M, Button{} }, { KEY_N, Button{} },
		  { KEY_O, Button{} }, { KEY_P, Button{} },
		  { KEY_Q, Button{} }, { KEY_R, Button{} },
		  { KEY_S, Button{} }, { KEY_T, Button{} },
		  { KEY_U, Button{} }, { KEY_V, Button{} },
		  { KEY_W, Button{} }, { KEY_X, Button{} },
		  { KEY_Y, Button{} }, { KEY_Z, Button{} },
		  { KEY_DELETE, Button{} }, { KEY_CAPSLOCK, Button{} },
		  { KEY_F1, Button{} }, { KEY_F2, Button{} },
		  { KEY_F3, Button{} }, { KEY_F4, Button{} },
		  { KEY_F5, Button{} }, { KEY_F6, Button{} },
		  { KEY_F7, Button{} }, { KEY_F8, Button{} },
		  { KEY_F9, Button{} }, { KEY_F10, Button{} },
		  { KEY_F11, Button{} }, { KEY_F12, Button{} },
		  { KEY_INSERT, Button{} }, { KEY_HOME, Button{} },
		  { KEY_PAGEUP, Button{} }, { KEY_END, Button{} },
		  { KEY_PAGEDOWN, Button{} },
		  { KEY_RIGHT, Button{} }, { KEY_LEFT, Button{} },
		  { KEY_DOWN, Button{} }, { KEY_UP, Button{} },
		  { KEY_NUMLOCK, Button{} },
		  { KEY_KP_DIVIDE, Button{} }, { KEY_KP_MULTIPLY, Button{} },
		  { KEY_KP_MINUS, Button{} }, { KEY_KP_PLUS, Button{} },
		  { KEY_KP_ENTER, Button{} },
		  { KEY_KP1, Button{} }, { KEY_KP2, Button{} },
		  { KEY_KP3, Button{} }, { KEY_KP4, Button{} },
		  { KEY_KP5, Button{} }, { KEY_KP6, Button{} },
		  { KEY_KP7, Button{} }, { KEY_KP8, Button{} },
		  { KEY_KP9, Button{} }, { KEY_KP0, Button{} },
		  { KEY_KP_PERIOD, Button{} },
		  { KEY_LCTRL, Button{} }, { KEY_LSHIFT, Button{} },
		  { KEY_LALT, Button{} }, { KEY_RCTRL, Button{} },
		  { KEY_RSHIFT, Button{} }, { KEY_RALT, Button{} } }
{
}

void Keyboard::Update()
{
	// Update key states from GLFW
	for ( auto& [key, button] : m_mapButtons )
	{
		button.Reset();
	}
}

void Keyboard::OnKeyPressed( int key )
{
	if ( key == KEY_UNKNOWN )
	{
		LOG_ERROR( "Key [{}] is unknown!", key );
		return;
	}
	auto keyItr = m_mapButtons.find( key );
	if ( keyItr == m_mapButtons.end() )
	{
		LOG_ERROR( "Key [{}] does not exist!", key );
		return;
	}
	keyItr->second.Update( true );
}

void Keyboard::OnKeyReleased( int key )
{
	if ( key == KEY_UNKNOWN )
	{
		LOG_ERROR( "Key [{}] is unknown!", key );
		return;
	}
	auto keyItr = m_mapButtons.find( key );
	if ( keyItr == m_mapButtons.end() )
	{
		LOG_ERROR( "Key [{}] does not exist!", key );
		return;
	}
	keyItr->second.Update( false );
}

const bool Keyboard::IsKeyPressed( int key ) const
{
	if ( key == KEY_UNKNOWN )
	{
		LOG_ERROR( "Key [{}] is unknown!", key );
		return false;
	}
	auto keyItr = m_mapButtons.find( key );
	if ( keyItr == m_mapButtons.end() )
	{
		LOG_ERROR( "Key [{}] does not exist!", key );
		return false;
	}
	return keyItr->second.bIsPressed;
}

const bool Keyboard::IsKeyJustPressed( int key ) const
{
	if ( key == KEY_UNKNOWN )
	{
		LOG_ERROR( "Key [{}] is unknown!", key );
		return false;
	}
	auto keyItr = m_mapButtons.find( key );
	if ( keyItr == m_mapButtons.end() )
	{
		LOG_ERROR( "Key [{}] does not exist!", key );
		return false;
	}
	return keyItr->second.bJustPressed;
}

const bool Keyboard::IsKeyJustReleased( int key ) const
{
	if ( key == KEY_UNKNOWN )
	{
		LOG_ERROR( "Key [{}] is unknown!", key );
		return false;
	}
	auto keyItr = m_mapButtons.find( key );
	if ( keyItr == m_mapButtons.end() )
	{
		LOG_ERROR( "Key [{}] does not exist!", key );
		return false;
	}
	return keyItr->second.bJustReleased;
}
