#pragma once

class Button;

class Keyboard
{
private:
	std::map<int, Button> m_mapButtons;
	GLFWwindow* m_window; // Add window handle

public:
	Keyboard();
	~Keyboard() = default;

	void Update();
	void OnKeyPressed( int key );
	void OnKeyReleased( int key );

	const bool IsKeyPressed( int key ) const;
	const bool IsKeyJustPressed( int key ) const;
	const bool IsKeyJustReleased( int key ) const;
};
