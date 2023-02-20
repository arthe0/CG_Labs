#pragma once

class Game;

class InputDevice
{

private:
	Game* game;
	int keys;

protected:
	virtual void OnKeyDown() = 0;
	virtual void OnMouseMove() = 0;

public:
	int MouseMove;
	int MouseOffset;
	int MousePosition;
	int MouseWheelID;
	void AddPressedKey();
	void IsKeyDown();
	void RemovePressed();
};
