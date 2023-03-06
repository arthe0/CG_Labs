#include "PinPong.h"
#include "Controls.h"
//#include "GeometryComponent.h"

PinPong::PinPong() : Engine(L"PinPong", 800, 800)
{
	gameFieldBox = DirectX::BoundingBox(DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3(1.f, 1.f, 1.f));

	players[Player::P_First] = new Racket(this, DirectX::XMFLOAT4(-0.8f, 0.f, 0.f, 0.f));
	players[Player::P_Second] = new Racket(this, DirectX::XMFLOAT4(0.8f, 0.f, 0.f, 0.f));

	ball = new Ball(this, DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f));

	Components.push_back(players[Player::P_First]);
	Components.push_back(players[Player::P_Second]);
	Components.push_back(ball);
}

PinPong::~PinPong()
{
	for (auto c : Components)
	{
		delete c;
	}
}

void PinPong::SetGameState(GameState newState)
{
	switch (newState)
	{
	case GS_PREMATCH:

		break;
	case GS_IN_PROGRESS:

		break;
	case GS_COOLDOWN:
		
		break;
	case GS_PAUSED:

		break;
	case GS_ENDED:

		break;
	}
}

void PinPong::OnPlayerScore(unsigned int points, Player player)
{
	playersScore[player] += points;

	if (playersScore[player] >= winScore)
		OnPlayerWin(player);

	SetGameState(GS_COOLDOWN);
}


void PinPong::OnPlayerWin(Player player)
{
	SetGameState(GS_ENDED);
}

void PinPong::OnCooldown()
{
	ball->SetPosition(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f));
}

#pragma region GameOverrided

void PinPong::Update()
{
	if(inputDevice)
	{
		if (inputDevice->IsKeyDown(DirectX::Keyboard::Escape))
			isExitRequested = true;
		if (inputDevice->IsKeyDown(Controls::Player1Up)) 
			players[Player::P_First]->Move(MD_UP, DeltaTime);
		if (inputDevice->IsKeyDown(Controls::Player1Down)) 
			players[Player::P_First]->Move(MD_DOWN, DeltaTime);
		if (inputDevice->IsKeyDown(Controls::Player2Up)) 
			players[Player::P_Second]->Move(MD_UP, DeltaTime);
		if (inputDevice->IsKeyDown(Controls::Player2Down)) 
			players[Player::P_Second]->Move(MD_DOWN, DeltaTime);
	}

	if(!ball->collision.Contains(gameFieldBox))
	{
		
		if(ball->collision.Center.x <= -1.f)
			OnPlayerScore(1, P_Second);
		else if (ball->collision.Center.x >= 1.f)
			OnPlayerScore(1, P_First);
		else
		{
			const DirectX::XMFLOAT2 direction = ball->GetDirection();
			ball->SetDirection(DirectX::XMFLOAT2(-direction.x, direction.y));
		}
			
	}
	
	Engine::Update();
}

void PinPong::UpdateInternal()
{
}

void PinPong::Draw()
{
	float color[] = { 0.f, 0.f, 0.f, 1.0f };
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



void PinPong::DestroyResources()
{
	for (auto c : Components)
	{
		c->DestroyResources();
	}
}
#pragma endregion GameOverrided