// MySuper3DApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "PinPong.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

int main()
{
	PinPong pinPong;
	pinPong.Run();
}
