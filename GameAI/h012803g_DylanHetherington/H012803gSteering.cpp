#include "H012803gSteering.h"



H012803gSteering::H012803gSteering(H012803gTank * directedTank)
{
	_pTank = directedTank;
	_resultantForce = Vector2D(0.0f, 0.0f);
	bool _seek = false;
	bool _flee = false;
	bool _arrive = false;
	bool _pursuit = false;
	bool _offsetPursuit = false;
	bool _evade = false;
	bool _wander = false;
	bool _obstacleAvoidance = false;
	_target = _pTank->GetPosition();
	_targetAgent = nullptr;
}
H012803gSteering::~H012803gSteering()
{
	delete _pTank;
	_tanksInView.clear();
}
Vector2D H012803gSteering::CalculateCumluativeForce()
{
	Vector2D force;
	if (_seek == true)
	{
		force += Seek(_target);
	}
	if (_flee == true)
	{
		force -= Flee(_target);
	}
	if (_arrive == true)
	{

	}
	if (_pursuit == true && _targetAgent != nullptr)
	{
		force += Pursuit(_targetAgent);
	}
	return force;
}
Vector2D H012803gSteering::Seek(Vector2D target)
{
	Vector2D velocity = Vec2DNormalize(target - _pTank->GetPosition()) * _pTank->GetMaxSpeed();
	return velocity - _pTank->GetVelocity();
}
Vector2D H012803gSteering::Flee(Vector2D target)
{
	Vector2D velocity = Vec2DNormalize(_pTank->GetPosition() - target)	* _pTank->GetMaxSpeed();
	return  velocity - _pTank->GetVelocity();
}
Vector2D H012803gSteering::Arrive(Vector2D target)
{
	Vector2D velocity;
	return  velocity;
}
Vector2D H012803gSteering::Pursuit(BaseTank* targetAgent)
{
	//if the evader is ahead and facing the agent then we can just seek
	//for the evader's current position.
	Vector2D ToEvader = targetAgent->GetPosition() - _pTank->GetPosition();

	double RelativeHeading = _pTank->GetHeading().Dot(targetAgent->GetHeading());

	if ((ToEvader.Dot(_pTank->GetHeading()) > 0) &&
		(RelativeHeading < -0.95))  //acos(0.95)=18 degs
	{
		return Seek(targetAgent->GetPosition());
	}
	//Not considered ahead so we predict where the evader will be.
	//the lookahead time is propotional to the distance between the evader
	//and the pursuer; and is inversely proportional to the sum of the
	//agent's velocities
	double LookAheadTime = ToEvader.Length() /	(_pTank->GetMaxSpeed() + targetAgent->GetCurrentSpeed());

	//now seek to the predicted future position of the evader
	_target = targetAgent->GetPosition() + targetAgent->GetVelocity() * LookAheadTime;
	return Seek(targetAgent->GetPosition() + targetAgent->GetVelocity() * LookAheadTime);
}
Vector2D H012803gSteering::OffsetPursuit(const BaseTank* targetAgent)
{
	Vector2D velocity;
	return  velocity;
}
Vector2D H012803gSteering::Evade(const BaseTank* targetAgent)
{
	Vector2D velocity;
	return  velocity;
}
Vector2D H012803gSteering::Wander()
{
	Vector2D velocity;
	return  velocity;
}
Vector2D H012803gSteering::ObstacleAvoidance()
{
	Vector2D velocity;
	return  velocity;
}
