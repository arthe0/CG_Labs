#include "PinPong.h"
#include "Controls.h"
#include <string>


PinPong::PinPong() : Engine(L"PinPong", 800, 800)
{
	gameFieldBox = DirectX::BoundingBox(DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3(1.f, 1.f, 1.f));

	players[Player::P_First] = new Racket(this, DirectX::XMFLOAT4(-0.8f, 0.f, 0.f, 0.f));
	players[Player::P_Second] = new Racket(this, DirectX::XMFLOAT4(0.8f, 0.f, 0.f, 0.f));

	ball = new Ball(this, DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f));

	Components.push_back(players[Player::P_First]);
	Components.push_back(players[Player::P_Second]);
	Components.push_back(ball);

	srand((unsigned)time(NULL));

	SetGameState(GS_PREMATCH);
}

PinPong::~PinPong()
{
	for (auto c : Components)
	{
		delete c;
	}
}

void PinPong::OnPlayerScore(unsigned int points, Player player)
{
	playersScore[player] += points;

	if (playersScore[player] >= winScore)
	{
		OnPlayerWin(player);
		return;
	}
		
	announcmentText = std::to_wstring(playersScore[Player::P_First]) + L"        " + std::to_wstring(playersScore[Player::P_Second]);
	SetGameState(GS_COOLDOWN);
}


void PinPong::OnPlayerWin(Player player)
{
	switch (player)
	{
	case P_First:
		announcmentText = L"Player 1 won";
		break;
	case P_Second:
		announcmentText = L"Player 2 won";
		break;
	}
	SetGameState(GS_ENDED);
}

void PinPong::SetGameState(GameState newState)
{
	currentState = newState;

	switch (newState)
	{
	case GS_PREMATCH:
		OnPreMatch();
		break;
	case GS_IN_PROGRESS:
		OnProgress();
		break;
	case GS_COOLDOWN:
		OnCooldown();
		break;
	case GS_PAUSED:
		OnPaused();
		break;
	case GS_ENDED:
		OnEnded();
		break;
	}
}

void PinPong::OnPreMatch()
{
	announcmentText = L"Press ENTER to start";
	EnableMovement(false);
}

void PinPong::OnProgress()
{
	announcmentText = std::to_wstring(playersScore[Player::P_First]) + L"        " + std::to_wstring(playersScore[Player::P_Second]);
	EnableMovement(true);
}

void PinPong::OnCooldown()
{
	ball->bCanMove = false;
	ball->GenerateRndDirection();
	continueTime = std::chrono::steady_clock::now() + std::chrono::seconds(2);
	ball->SetPosition(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f));
	ball->ResetSpeed();
}

void PinPong::OnPaused()
{
	announcmentText = L"Paused";
	EnableMovement(false);
}

void PinPong::OnEnded()
{
	EnableMovement(false);
}

void PinPong::ReleasePause()
{
	continueTime = std::chrono::steady_clock::now() + std::chrono::seconds(2);
}

void PinPong::EnableMovement(bool bCanMove)
{
	players[Player::P_First]->bCanMove = bCanMove;
	players[Player::P_Second]->bCanMove = bCanMove;
	ball->bCanMove = bCanMove;
}

#pragma region EngineOverrided

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

		if (inputDevice->IsKeyDown(DirectX::Keyboard::Enter))
		{
			switch (currentState)
			{
			case GS_IN_PROGRESS:
				SetGameState(GS_PAUSED);
				break;
			case GS_PREMATCH:
				SetGameState(GS_COOLDOWN);
				break;
			case GS_PAUSED:
				ReleasePause();
				break;
			case GS_ENDED:
				SetGameState(GS_PREMATCH);
				break;
			}
		}
	}
	if (currentState == GS_PAUSED && std::chrono::steady_clock::now() <= continueTime)
	{
		announcmentText = std::to_wstring(std::chrono::duration_cast<std::chrono::seconds>(continueTime - std::chrono::steady_clock::now()).count());
	}
	
	if ((currentState == GS_COOLDOWN || currentState == GS_PAUSED) && std::chrono::steady_clock::now() >= continueTime)
	{
		SetGameState(GS_IN_PROGRESS);
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
			ball->SetDirection(DirectX::XMFLOAT2(direction.x, -direction.y));
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
	
	D2DRenderTarget->BeginDraw();
	D2DRenderTarget->DrawText(
		announcmentText.c_str(),
		announcmentText.length(),
		DWTextFormat,
		D2D1::RectF(0, -700, 800, 800),
		D2Dbrush
	);
	D2DRenderTarget->DrawLine(
		D2D1::Point2F(400, -800),
		D2D1::Point2F(400, 800),
		D2Dbrush,
		1.0f,
		D2DLineStrokeStyle
	);
	D2DRenderTarget->EndDraw();

	Engine::Draw();
}

void PinPong::EndFrame()
{
}

void PinPong::Initialize()
{
	static const WCHAR msc_fontName[] = L"Impact";
	static const FLOAT msc_fontSize = 50;
	HRESULT hr;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2DFactory);

	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite factory.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(DWFactory),
			reinterpret_cast<IUnknown**>(&DWFactory)
		);
	}
	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = DWFactory->CreateTextFormat(
			msc_fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			msc_fontSize,
			L"", //locale
			&DWTextFormat
		);
	}
	if (SUCCEEDED(hr))
	{
		// Center the text horizontally and vertically.
		DWTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

		DWTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	hr = SwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&D2DBackBuff);

	// Create the DXGI Surface Render Target.
	float dpi = GetDpiForWindow(Display->hWnd);

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			0.0f,
			0.0f);

	// Create a Direct2D render target that can draw into the surface in the swap chain

	hr = D2DFactory->CreateDxgiSurfaceRenderTarget(
		D2DBackBuff,
		&props,
		&D2DRenderTarget);

	hr = D2DRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White, 1.0f),
		&D2Dbrush
	);

	hr = D2DFactory->CreateStrokeStyle(
		D2D1::StrokeStyleProperties(
			D2D1_CAP_STYLE_FLAT,
			D2D1_CAP_STYLE_FLAT,
			D2D1_CAP_STYLE_ROUND,
			D2D1_LINE_JOIN_MITER,
			10.0f,
			D2D1_DASH_STYLE_DASH,
			0.0f),
		nullptr,
		0,
		&D2DLineStrokeStyle
	);

	Engine::Initialize();
}

void PinPong::PrepareFrame()
{
}

void PinPong::PrepareResources()
{
}

void PinPong::DestroyResources()
{
	D2DFactory->Release();
	DWFactory->Release();
	DWTextFormat->Release();
	D2DRenderTarget->Release();
	D2Dbrush->Release();
	D2DLineStrokeStyle->Release();
	D2DBackBuff->Release();

	Engine::DestroyResources();
}
#pragma endregion EngineOverrided