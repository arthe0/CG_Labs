#pragma once

class Engine;

class GameComponent
{
public:
	Engine* engine;
	GameComponent(Engine* e);
	virtual ~GameComponent();

	virtual void DestroyResources() = 0;
	virtual void Draw() = 0;
	virtual void Initialize() = 0;
	virtual void Reload() = 0;
	virtual void Update() = 0;
};
