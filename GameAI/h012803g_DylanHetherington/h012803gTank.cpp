#include "h012803gTank.h"
#include "../TankManager.h"
#include "../Commons.h"
#include "../C2DMatrix.h"

//--------------------------------------------------------------------------------------------------

H012803gTank::H012803gTank(SDL_Renderer* renderer, TankSetupDetails details) 
	: BaseTank(renderer, details)
{
	mTankTurnDirection  = DIRECTION_UNKNOWN;
	mTankMoveDirection	= DIRECTION_NONE;
	mManTurnDirection   = DIRECTION_UNKNOWN;
}

//--------------------------------------------------------------------------------------------------

H012803gTank::~H012803gTank()
{
}

//--------------------------------------------------------------------------------------------------

void H012803gTank::ChangeState(BASE_TANK_STATE newState)
{
	BaseTank::ChangeState(newState);
}

//--------------------------------------------------------------------------------------------------

void H012803gTank::Update(float deltaTime, SDL_Event e)
{
	switch (e.type)
	{
	case SDL_MOUSEBUTTONDOWN: 
		SDL_GetMouseState(&mouseCoords.x, &mouseCoords.y);
		break;
	}
}

void H012803gTank::Render()
{
	Vector2D circleVector(mouseCoords.x, mouseCoords.y);
	DrawDebugCircle(circleVector, 5.0f, 255, 100, 1);
	GameObject::Render();
	BaseTank::Render();
}
//--------------------------------------------------------------------------------------------------

void H012803gTank::MoveInHeadingDirection(float deltaTime)
{
	//Get the force that propels in current heading.
	Vector2D force = (mHeading*mCurrentSpeed)-mVelocity;

	//Acceleration = Force/Mass
	Vector2D acceleration = force/GetMass();

	//Update velocity.
	mVelocity += acceleration * deltaTime;
	
	//Don't allow the tank does not go faster than max speed.
	mVelocity.Truncate(GetMaxSpeed()); //TODO: Add Penalty for going faster than MAX Speed.

	//Finally, update the position.
	Vector2D newPosition = GetPosition();
		newPosition.x += mVelocity.x*deltaTime;
		newPosition.y += (mVelocity.y/**-1.0f*/)*deltaTime;	//Y flipped as adding to Y moves down screen.
	SetPosition(newPosition);
}

//--------------------------------------------------------------------------------------------------

void H012803gTank::RotateHeadingByRadian(double radian, int sign)
{
	//Clamp the amount to turn to the max turn rate.
	if (radian > mMaxTurnRate) 
		radian = mMaxTurnRate;
	else if(radian < -mMaxTurnRate)
		radian = -mMaxTurnRate;
	//IncrementTankRotationAngle(RadsToDegs(radian));
    mRotationAngle += RadsToDegs(radian)*sign;

	//Usee a rotation matrix to rotate the player's heading
	C2DMatrix RotationMatrix;
  
	//Calculate the direction of rotation.
	RotationMatrix.Rotate(radian * sign);	
	//Get the new heading.
	RotationMatrix.TransformVector2Ds(mHeading);

	//cout << "RotateHeadingByRadian -- Heading x = " << mHeading.x << " y = " << mHeading.y << endl;

	//Get the new velocity.
	RotationMatrix.TransformVector2Ds(mVelocity);

	//Side vector must always be perpendicular to the heading.
	mSide = mHeading.Perp();
}

//--------------------------------------------------------------------------------------------------