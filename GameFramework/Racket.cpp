#include "Racket.h"

Racket::Racket(Engine* e, DirectX::XMFLOAT4 position) 
	: GeometryComponent(e, Shapes::S_Raket, position)
{
	collision = DirectX::BoundingBox (DirectX::XMFLOAT3(GetPosX(), GetPosY(), GetPosZ()), DirectX::XMFLOAT3());
}

Racket::~Racket()
{
	
}

void Racket::Move(MovingDirection Direction, float DeltaTime)
{
	float DeltaY = speed * DeltaTime;
	switch (Direction)
	{
	case MD_UP:
		
		SetPosY(GetPosY() + DeltaY);
		collision.Center.y += DeltaY;
		break;

	case MD_DOWN:

		SetPosY(GetPosY() - DeltaY);
		collision.Center.y -= DeltaY;
		break;
	}
}
