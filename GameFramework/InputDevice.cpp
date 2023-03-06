#include "InputDevice.h"
#include "Engine.h"

InputDevice::InputDevice(Engine* e)
{
	keyboard = std::make_unique<DirectX::Keyboard>();
	MousePosition.x = 0;
	MousePosition.y = 0;
}

bool InputDevice::IsKeyDown(DirectX::Keyboard::Keys key)
{
	if(!keyboard) return false;

	bool result = keyboard->GetState().IsKeyDown(key);
	if(result)
	{
		int a = 0;
	}
	return result;
}

bool InputDevice::IsKeyUp(DirectX::Keyboard::Keys key)
{
	if (!keyboard) return false;

	auto result = keyboard->GetState().IsKeyUp(key);
	return keyboard->GetState().IsKeyUp(key);
}

void InputDevice::RemovePressed()
{
}
