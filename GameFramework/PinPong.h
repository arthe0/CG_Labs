#include "Engine.h"
#include "GeometryComponent.h"
#include "Racket.h"
#include "Ball.h"


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
protected:
	void SetGameState(GameState newState);
	
	void DestroyResources() override;
	void Draw() override;
	void EndFrame() override;
	void Initialize() override;
	void PrepareFrame() override;
	void PrepareResources() override;
	void Update() override;
	void UpdateInternal() override;

private:
	unsigned int playersScore [Player::P_MAX] {0};
	unsigned int winScore = 10;

	Racket* players [Player::P_MAX];
	Ball* ball;

	void OnPlayerScore(unsigned int points, Player player);
	void OnPlayerWin(Player player);
	void OnCooldown();
};