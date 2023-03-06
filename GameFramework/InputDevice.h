#pragma once
#include "Keyboard.h"
#include <DirectXMath.h>

class Engine;

class InputDevice
{

public:
	InputDevice(Engine* e);

	DirectX::XMINT2 MousePosition;

	bool IsKeyDown(DirectX::Keyboard::Keys key);
	bool IsKeyUp(DirectX::Keyboard::Keys key);
	void RemovePressed();

private:
	Engine* engine;
	std::unique_ptr<DirectX::Keyboard> keyboard;
};
