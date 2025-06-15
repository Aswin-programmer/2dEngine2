#include "pch.h"

#include "WINDOW/INPUT/Mouse.h"

#include "LOGGER/log.h"
#include "WINDOW/Window.h"

void Mouse::Update()
{
	for ( auto& [btn, button] : m_mapButtons )
		button.Reset();
	m_WheelX = 0;
	m_WheelY = 0;
	m_bMouseMoving = false;
}

void Mouse::OnBtnPressed( int btn )
{
	if ( btn == MOUSE_UNKNOWN )
	{
		LOG_ERROR( "Mouse Button [{}] is unknown!", btn );
		return;
	}
	auto btnItr = m_mapButtons.find( btn );
	if ( btnItr == m_mapButtons.end() )
	{
		LOG_ERROR( "Mouse Button [{}] does not exist!", btn );
		return;
	}
	btnItr->second.Update( true );
}

void Mouse::OnBtnReleased( int btn )
{
	if ( btn == MOUSE_UNKNOWN )
	{
		LOG_ERROR( "Mouse Button [{}] is unknown!", btn );
		return;
	}
	auto btnItr = m_mapButtons.find( btn );
	if ( btnItr == m_mapButtons.end() )
	{
		LOG_ERROR( "Mouse Button [{}] does not exist!", btn );
		return;
	}
	btnItr->second.Update( false );
}

const bool Mouse::IsBtnPressed( int btn ) const
{
	if ( btn == MOUSE_UNKNOWN )
	{
		LOG_ERROR( "Mouse Button [{}] is unknown!", btn );
		return false;
	}
	auto btnItr = m_mapButtons.find( btn );
	if ( btnItr == m_mapButtons.end() )
	{
		LOG_ERROR( "Mouse Button [{}] does not exist!", btn );
		return false;
	}
	return btnItr->second.bIsPressed;
}

const bool Mouse::IsBtnJustPressed( int btn ) const
{
	if ( btn == MOUSE_UNKNOWN )
	{
		LOG_ERROR( "Mouse Button [{}] is unknown!", btn );
		return false;
	}
	auto btnItr = m_mapButtons.find( btn );
	if ( btnItr == m_mapButtons.end() )
	{
		LOG_ERROR( "Mouse Button [{}] does not exist!", btn );
		return false;
	}
	return btnItr->second.bJustPressed;
}

const bool Mouse::IsBtnJustReleased( int btn ) const
{
	if ( btn == MOUSE_UNKNOWN )
	{
		LOG_ERROR( "Mouse Button [{}] is unknown!", btn );
		return false;
	}
	auto btnItr = m_mapButtons.find( btn );
	if ( btnItr == m_mapButtons.end() )
	{
		LOG_ERROR( "Mouse Button [{}] does not exist!", btn );
		return false;
	}
	return btnItr->second.bJustReleased;
}

const std::tuple<int, int> Mouse::GetMouseScreenPosition()
{
	glfwGetCursorPos( Window::getGLFWWindow(), &m_X, &m_Y );
	return std::make_tuple( m_X, m_Y );
}
