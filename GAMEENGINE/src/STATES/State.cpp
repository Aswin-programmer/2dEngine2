#include "pch.h"

#include "STATES/State.h"

#include "LOGGER/log.h"

void State::CreateLuaStateBind( sol::state& lua )
{
	lua.new_usertype<State>(
		"State",
		sol::call_constructor,
		sol::factories(
			[]( const std::string& name ) {
				return State{ .name = name };
			},
			[]( const std::string& name, sol::protected_function on_enter, sol::protected_function on_exit,
				sol::protected_function on_update, sol::protected_function on_render, sol::protected_function handle_inputs,
				sol::object variables ) {
				return State{
					.name = name,
					.on_render = on_render,
					.on_update = on_update,
					.on_enter = on_enter,
					.on_exit = on_exit,
					.handle_inputs = handle_inputs,
					.variables = variables };
			} ),
		"set_on_enter", []( State& state, sol::protected_function on_enter ) {
			if (!on_enter.valid())
			{
				LOG_ERROR("Failed to set the on_enter function");
				return;
			}
			state.on_enter = on_enter; },
		"set_on_exit", []( State& state, sol::protected_function on_exit ) {
			if (!on_exit.valid())
			{
				LOG_ERROR("Failed to set the on_exit function");
				return;
			}
			state.on_exit = on_exit; },
		"set_on_update", []( State& state, sol::protected_function on_update ) {
			if (!on_update.valid())
			{
				LOG_ERROR("Failed to set the on_update function");
				return;
			}
			state.on_update = on_update; },
		"set_on_render", []( State& state, sol::protected_function on_render ) {
			if (!on_render.valid())
			{
				LOG_ERROR("Failed to set the on_render function");
				return;
			}
			state.on_render = on_render; },
		"set_handle_inputs", []( State& state, sol::protected_function handle_inputs ) {
			if (!handle_inputs.valid())
			{
				LOG_ERROR("Failed to set the handle_inputs function");
				return;
			}
			state.handle_inputs = handle_inputs; },
		"set_variable_table", []( State& state, const sol::table& table ) {
			if (!table.valid())
			{
				LOG_ERROR("Failed to set the variables function");
				return;
			}
			state.variables = table; },
		"variables", &State::variables,
		"name", &State::name,
		"bKillState", &State::bKillState );
}
