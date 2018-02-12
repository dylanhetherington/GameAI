#ifndef H012803GTANK
#define H012803GTANK

#include "../BaseTank.h"
#include <SDL.h>
#include "../Commons.h"
#include "H012803gSteering.h"

//---------------------------------------------------------------
class H012803gSteering;
class H012803gTank : public BaseTank
{
	//---------------------------------------------------------------
public:
	H012803gTank(SDL_Renderer* renderer, TankSetupDetails details);
	~H012803gTank();

	void ChangeState(BASE_TANK_STATE newState);

	void Update(float deltaTime, SDL_Event e);
	void Render();
	void RotateHeadingByRadian(double radian, int sign, float deltaTime);
	bool RotateHeadingToFacePosition(Vector2D, float);

	//---------------------------------------------------------------
protected:
	void	MoveInHeadingDirection(float deltaTime);

private:
	SDL_Point mouseCoords;
	TURN_DIRECTION  mTankTurnDirection;
	MOVE_DIRECTION  mTankMoveDirection;
	TURN_DIRECTION  mManTurnDirection;
	H012803gSteering* _pSteeringBehaviour;

	bool _behind;
};

//---------------------------------------------------------------

#endif //CONTROLLEDTANK_H