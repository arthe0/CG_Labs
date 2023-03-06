#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "GeometryComponent.h"


enum MovingDirection
{
    MD_UP,
    MD_DOWN
};

class Racket : public GeometryComponent
{
public:
    Racket(Engine* e, DirectX::XMFLOAT4 position);
    ~Racket();

    void Move(MovingDirection direction, float deltaTime);
    float speed = 1.f;

    DirectX::BoundingBox collision;
};

