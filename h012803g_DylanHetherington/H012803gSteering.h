#pragma once
#include "../Commons.h"
#include "h012803gTank.h"
#include "../BaseTank.h"

class H012803gSteering
{
public:
	H012803gSteering();
	~H012803gSteering();

private:
	Vector2D _resultantForce;
	H012803gTank* _tank;
	Vector2D _target;

	Vector2D Seek(Vector2D target);
	Vector2D Flee(Vector2D target);
	Vector2D Arrive(Vector2D target);
	Vector2D Pursuit(const BaseTank* targetAgent);
	Vector2D OffsetPursuite(const BaseTank* targetAgent);
	Vector2D Evade(const BaseTank* targetAgent);
	Vector2D Wander();
	Vector2D ObstacleAvoidance(); 
};

