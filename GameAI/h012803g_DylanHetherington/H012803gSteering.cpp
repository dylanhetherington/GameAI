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
	_target = Vector2D(0.0f, 0.0f);
}
H012803gSteering::~H012803gSteering()
{
	delete _pTank;
	_tanksInView.clear();
}
Vector2D H012803gSteering::CalculateCumluativeForce()
{
	Vector2D force;
	if (_seek = true)
	{
		force = Seek(_target);
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
	Vector2D velocity;
	return  velocity;
}
Vector2D H012803gSteering::Arrive(Vector2D target)
{
	Vector2D velocity;
	return  velocity;
}
Vector2D H012803gSteering::Pursuit(const BaseTank* targetAgent)
{
	Vector2D velocity;
	return  velocity;
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
