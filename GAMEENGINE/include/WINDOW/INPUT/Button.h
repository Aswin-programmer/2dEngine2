#pragma once

struct Button
{
	bool bIsPressed{ false }, bJustPressed{ false }, bJustReleased{ false };
	void Update( bool bPressed )
	{
		bJustPressed = !bIsPressed && bPressed;
		bJustReleased = bIsPressed && !bPressed;
		bIsPressed = bPressed;
	}
	void Reset()
	{
		bJustPressed = false;
		bJustReleased = false;
	}
};
