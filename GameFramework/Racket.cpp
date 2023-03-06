#include "Racket.h"

Racket::Racket(Engine* e, DirectX::XMFLOAT4 position) 
	: GeometryComponent(e, Shapes::S_Raket, position)
{
	collision = DirectX::BoundingBox 
	(
		DirectX::XMFLOAT3(GetPosX(), GetPosY(), GetPosZ()), 
		DirectX::XMFLOAT3(Shape.Points[0].x, Shape.Points[0].y, 0.f)
	);
}

Racket::~Racket()
{
	
}

void Racket::Move(MovingDirection Direction, float DeltaTime)
{
	if(!bCanMove) return;

	float DeltaY = speed * DeltaTime;
	switch (Direction)
	{
	case MD_UP:
		
		if(GetPosY() + DeltaY + Shape.Points[0].y > 0.95f ) return;
		SetPosY(GetPosY() + DeltaY);
		collision.Center.y += DeltaY;
		break;

	case MD_DOWN:
		if (GetPosY() - DeltaY - Shape.Points[0].y < -0.95f) return;
		SetPosY(GetPosY() - DeltaY);
		collision.Center.y -= DeltaY;
		break;
	}
}
