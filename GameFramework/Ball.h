#pragma once
#include "GeometryComponent.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>

class PinPong;

class Ball : public GeometryComponent
{
public:
	Ball(Engine* e, DirectX::XMFLOAT4 position);

	DirectX::BoundingSphere collision;

	void IncreaseSpeed();
	void Move(float deltaTime);
	void GenerateRndDirection();
	void ResetSpeed();

	void Update() override;
	
	void SetPosition(DirectX::XMFLOAT4 NewPosition) override;
	void SetDirection(DirectX::XMFLOAT2 NewDirection);
	DirectX::XMFLOAT2 GetDirection () const;

	bool bCanMove = false;
protected:


private:
	PinPong* pinPong;

	float baseSpeed = 0.7f;
	float speed = 0.7f;
	float speedIncrement = 0.1f;
	DirectX::XMFLOAT2 direction;
	
};

