#include "Ball.h"
#include "Engine.h"
#include <stdlib.h>
#include "PinPong.h"


Ball::Ball(Engine* e, DirectX::XMFLOAT4 position)
	: GeometryComponent(e, Shapes::S_Square, position)
{
	pinPong = static_cast<PinPong*>(e);

	collision = DirectX::BoundingSphere
	(
		DirectX::XMFLOAT3(GetPosX(), GetPosY(), GetPosZ()), 
		0.01f
	);
	
	GenerateRndDirection();
}

void Ball::IncreaseSpeed()
{
	
	speed += speedIncrement;
}

void Ball::GenerateRndDirection()
{	
	using namespace DirectX;
	XMVECTOR result = XMVectorLerp(
		XMLoadFloat2(new XMFLOAT2(1.f, 1.f)),
		XMLoadFloat2(new XMFLOAT2(1.f, -1.f)),
		(float)rand() / RAND_MAX
	);

	XMVector2Normalize(result);
	XMStoreFloat2(&direction, result);
	if ((float)rand() / RAND_MAX < 0.5f)
		direction.x *= -1;
}

void Ball::ResetSpeed()
{
	speed = baseSpeed;
}

void Ball::Update()
{
	if (bCanMove) Move(engine->DeltaTime);

	using namespace DirectX;
	if(pinPong->players[Player::P_First]->collision.Intersects(collision))
	{
		XMVECTOR playerCenter = XMLoadFloat4(&pinPong->players[Player::P_First]->position);
		XMVECTOR ballDirection = XMLoadFloat2(&direction);
		XMVECTOR ballCenter = XMLoadFloat4(&position);

		XMFLOAT2 temp;
		XMVECTOR newDirection = -playerCenter + ballCenter;
		XMVector2Normalize(newDirection);
		XMStoreFloat2(&temp, newDirection);
		temp.y = std::clamp(temp.y, -0.02f, 0.02f);
		SetDirection(temp);

		IncreaseSpeed();
	}
	if(pinPong->players[Player::P_Second]->collision.Intersects(collision))
	{
		XMVECTOR playerCenter = XMLoadFloat4(&pinPong->players[Player::P_Second]->position);
		XMVECTOR ballDirection = XMLoadFloat2(&direction);
		XMVECTOR ballCenter = XMLoadFloat4(&position);

		XMFLOAT2 temp;
		XMVECTOR newDirection = -playerCenter + ballCenter;
		XMVector2Normalize(newDirection);
		XMStoreFloat2(&temp, newDirection);
		temp.y = std::clamp(temp.y, -0.02f, 0.02f);
		SetDirection(temp);

		IncreaseSpeed();
	}
	

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
