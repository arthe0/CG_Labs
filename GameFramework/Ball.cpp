#include "Ball.h"
#include "Engine.h"
#include <stdlib.h>


Ball::Ball(Engine* e, DirectX::XMFLOAT4 position)
	: GeometryComponent(e, Shapes::S_Square, position), engine(e)
{
	collision = DirectX::BoundingSphere(DirectX::XMFLOAT3(GetPosX(), GetPosY(), GetPosZ()), 0.01f);
	GenerateRndDirection();

	srand(time(NULL));
}

void Ball::IncreaseSpeed()
{
	speed += speedIncrement;
}

void Ball::GenerateRndDirection()
{	
	using namespace DirectX;
	XMVECTOR result = XMVectorLerp(
		XMLoadFloat2(new XMFLOAT2(1.f, -1.f)), 
		XMLoadFloat2(new XMFLOAT2(1.f, 1.f)), 
		(float)rand() / RAND_MAX
	);

	XMVector2Normalize(result);
	XMStoreFloat2(&direction, result);
}

void Ball::Update()
{
	if (bCanMove) Move(engine->DeltaTime);

	GeometryComponent::Update();
}

void Ball::SetPosition(DirectX::XMFLOAT4 NewPosition)
{
	collision.Center.x = NewPosition.x;
	collision.Center.y = NewPosition.y;
	collision.Center.z = NewPosition.z;

	GeometryComponent::SetPosition(NewPosition);
}

void Ball::Move(float deltaTime)
{
	SetPosX(GetPosX() + direction.x * speed * deltaTime);
	SetPosY(GetPosY() + direction.y * speed * deltaTime);
	collision.Center.x = GetPosX();
	collision.Center.y = GetPosY();
}

void Ball::SetDirection(DirectX::XMFLOAT2 NewDirection)
{
	DirectX::XMStoreFloat2(&direction, DirectX::XMVector2Normalize(DirectX::XMLoadFloat2(&NewDirection)));
}

DirectX::XMFLOAT2 Ball::GetDirection() const
{
	return direction;
}
