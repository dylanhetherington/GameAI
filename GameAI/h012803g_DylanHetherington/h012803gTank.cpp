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
	_pSteeringBehaviour = new H012803gSteering(this);
	_pSteeringBehaviour->ToggleSeek();
	_pSteeringBehaviour->ToggleObstacleAvoidance();
	//_pSteeringBehaviour->ToggleFlee();
	//_pSteeringBehaviour->TogglePursuit();
	//_pSteeringBehaviour->ToggleArrive();
	_behind = false;
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
		_pSteeringBehaviour->SetTarget(mouseCoords.x, mouseCoords.y);
		break;
	}

	mTanksICanSee = TankManager::Instance()->GetVisibleTanks(this);
	_enemyMines = ProjectileManager::Instance()->GetVisibleMines(this);
	for (int i = 0; i < _enemyMines.size(); i++)
	{
		_pSteeringBehaviour->SetDangerousMine(_enemyMines[i]);
	}
	if (_enemyMines.size() == 0)
	{
		_pSteeringBehaviour->SetDangerousMine(nullptr);
	}
	for (int i = 0; i < mTanksICanSee.size(); i++)
	{
		_pSteeringBehaviour->SetTargetAgent(mTanksICanSee[i]);
	}
	if (mTanksICanSee.size() == 0)
	{
		_pSteeringBehaviour->SetTargetAgent(nullptr);
	}
	RotateHeadingToFacePosition(_pSteeringBehaviour->GetTarget(), deltaTime);
	MoveInHeadingDirection(deltaTime);
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
	//Get the force that propels in current heading. = (mHeading*mCurrentSpeed)-mVelocity;
	Vector2D force;
		if (_behind)
		{
			force = mHeading.GetReverse() * _pSteeringBehaviour->CalculateCumluativeForce().Length() - mVelocity;
		}
		else
		{
			force = mHeading * _pSteeringBehaviour->CalculateCumluativeForce().Length() - mVelocity;
		}
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
bool H012803gTank::RotateHeadingToFacePosition(Vector2D target, float deltaTime)
{
	Vector2D toTarget;
	if (_pSteeringBehaviour->GetFlee() == true)
	{
		toTarget = Vec2DNormalize(target - GetCentralPosition());
	}
	else
	{
		toTarget = Vec2DNormalize(GetCentralPosition() - target);
	}


	//Determine the angle between the heading vector and the target.
	double angle = acos(mHeading.Dot(toTarget));

	//Ensure angle does not become NaN and cause the tank to disappear.
	if (angle != angle)
		angle = 0.0f;

	//Return true if the player is facing the target.
	if (angle < 0.00001)
		return true;
	if (angle <= 1.571)
	{
		_behind = true;
	}
	else
	{
		_behind = false;
	}
	RotateHeadingByRadian(angle, mHeading.Sign(toTarget), deltaTime);

	return true;
}
void H012803gTank::RotateHeadingByRadian(double radian, int sign, float deltaTime)
{
	//Incorporate delta time.
	radian *= deltaTime;
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