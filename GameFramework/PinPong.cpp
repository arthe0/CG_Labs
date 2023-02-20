#include "PinPong.h"

PinPong::PinPong() : Game(L"PinPong", 800, 800)
{

}

PinPong::~PinPong()
{
	for (auto c : Components)
	{
		delete c;
	}
}

void PinPong::DestroyResources()
{
	for (auto c : Components)
	{
		c->DestroyResources();
	}
}

void PinPong::Draw()
{
	float color[] = { TotalTime, 0.1f, 0.1f, 1.0f };
	Context->ClearRenderTargetView(RenderView, color);

	for (auto c : Components)
	{
		c->Draw();
	}
}

void PinPong::EndFrame()
{
}

void PinPong::Initialize()
{
	Components.push_back(new TriangleComponent(this));

	for (auto c : Components)
	{
		c->Initialize();
	}
}

void PinPong::PrepareFrame()
{
}

void PinPong::PrepareResources()
{
}

void PinPong::Update()
{
}

void PinPong::UpdateInternal()
{
}
