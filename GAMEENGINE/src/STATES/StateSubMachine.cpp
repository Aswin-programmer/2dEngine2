#include "pch.h"

#include "STATES/StateSubMachine.h"

#include "LOGGER/log.h"
#include "STATES/State.h"

StateSubMachine::StateSubMachine()
	: StateSubMachine( sol::lua_nil_t{} )
{
}

StateSubMachine::StateSubMachine( const sol::table& stateFuncs )
	: m_mapStates{}, m_StateTable{ stateFuncs }
{
}

void StateSubMachine::Update( const float dt )
{
	for ( auto& pair : m_mapStates )
	{
		try
		{
			auto stateItr = m_mapStates.find( pair.first );
			if ( stateItr == m_mapStates.end() )
				return;

			if ( stateItr->second->on_update.valid() )
			{
				auto result = stateItr->second->on_update( dt );
				if ( !result.valid() )
				{
					sol::error error = result;
					throw error;
				}
			}

			// Check for states to remove
			std::erase_if( m_mapStates, []( auto& state ) { return state.second->bKillState; } );
		}
		catch ( const sol::error& error )
		{
			LOG_ERROR( "Failed to update state: {}", error.what() );
		}
		catch ( ... )
		{
			LOG_ERROR( "Failed to update state: Unknown error." );
		}
	}
}

void StateSubMachine::Render()
{
	for ( auto& pair : m_mapStates )
	{
		try
		{
			auto stateItr = m_mapStates.find( pair.first );
			if ( stateItr == m_mapStates.end() )
				return;

			if ( stateItr->second->on_render.valid() )
			{
				auto result = stateItr->second->on_render();
				if ( !result.valid() )
				{
					sol::error error = result;
					throw error;
				}
			}
		}
		catch ( const sol::error& error )
		{
			LOG_ERROR( "Failed to render state: {}", error.what() );
		}
		catch ( ... )
		{
			LOG_ERROR( "Failed to render state: Unknown error." );
		}
	}
}

void StateSubMachine::AddState( const State& state )
{
	if ( m_mapStates.contains( state.name ) )
	{
		LOG_ERROR( "Failed to add state: {} -- Already exists.", state.name );
		return;
	}

	m_mapStates.emplace( state.name, std::make_unique<State>( state ) );
	auto newState = m_mapStates.find( state.name );
	if ( newState == m_mapStates.end() )
		return;
	if ( newState->second->on_enter.valid() )
	{
		try
		{
			auto result = newState->second->on_enter();
			if ( !result.valid() )
			{
				sol::error error = result;
				throw error;
			}
		}
		catch ( const sol::error& error )
		{
			LOG_ERROR( "Failed to enter state: {}", error.what() );
			return;
		}
	}
}

void StateSubMachine::ExitState( const State& state )
{
	auto stateItr = m_mapStates.find( state.name );
	if ( stateItr == m_mapStates.end() )
	{
		LOG_ERROR( "Failed to exit state: {} -- State does not exist.", state.name );
		return;
	}

	stateItr->second->on_exit();
	stateItr->second->bKillState = true;
}

void StateSubMachine::DestroyStates()
{
	for ( auto& [name, state] : m_mapStates )
	{
		state->on_exit();
	}

	m_mapStates.clear();
}

void StateSubMachine::CreateLuaStateSubMachine( sol::state& lua )
{
	lua.new_usertype<StateSubMachine>(
		"StateSubMachine",
		sol::call_constructor,
		sol::constructors<StateSubMachine(), StateSubMachine( const sol::table& )>(),
		"update",
		&StateSubMachine::Update,
		"render",
		&StateSubMachine::Render,
		"add_state",
		&StateSubMachine::AddState,
		"exit_state",
		&StateSubMachine::ExitState,
		"destroy",
		&StateSubMachine::DestroyStates );
}
