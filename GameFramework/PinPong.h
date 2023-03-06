#include "Engine.h"
#include "GeometryComponent.h"
#include "Racket.h"
#include "Ball.h"
#include <d2d1.h>
#include <dwrite.h>


enum GameState
{
	GS_PREMATCH,
	GS_IN_PROGRESS,
	GS_COOLDOWN,
	GS_PAUSED,
	GS_ENDED
};

enum Player
{
	P_First,
	P_Second,
	P_MAX
};

class PinPong : public Engine
{

public:
	PinPong();
	~PinPong();

	DirectX::BoundingBox gameFieldBox;
	std::chrono::steady_clock::time_point continueTime;

	Racket* players[Player::P_MAX];
protected:

	ID2D1Factory* D2DFactory;
	IDWriteFactory* DWFactory;
	IDWriteTextFormat* DWTextFormat;
	ID2D1RenderTarget* D2DRenderTarget;
	ID2D1SolidColorBrush* D2Dbrush;
	ID2D1StrokeStyle* D2DLineStrokeStyle;
	IDXGISurface* D2DBackBuff;

	void SetGameState(GameState newState);
	
	void DestroyResources() override;
	void Draw() override;
	void EndFrame() override;
	virtual void Initialize() override;
	void PrepareFrame() override;
	void PrepareResources() override;
	virtual void Update() override;
	void UpdateInternal() override;

private:
	unsigned int playersScore [Player::P_MAX] {0};
	unsigned int winScore = 10;

	GameState currentState;


	Ball* ball;

	std::wstring announcmentText;

	void OnPlayerScore(unsigned int points, Player player);
	void OnPlayerWin(Player player);

	void OnPreMatch();
	void OnProgress();
	void OnCooldown();
	void OnPaused();
	void OnEnded();

	void ReleasePause();
	void EnableMovement(bool bCanMove);
};