#pragma once

struct State;

class StateSubMachine
{
private:
	std::map<std::string, std::unique_ptr<State>> m_mapStates;
	std::optional<sol::table> m_StateTable;

public:
	StateSubMachine();
	StateSubMachine( const sol::table& stateFuncs );

	void Update( const float dt );
	void Render();
	void AddState( const State& state );
	void ExitState( const State& state );
	void DestroyStates();

	static void CreateLuaStateSubMachine( sol::state& lua );
};
