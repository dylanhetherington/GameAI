#pragma once
#include "../Commons.h"
#include "h012803gTank.h"
#include "../BaseTank.h"
#include <vector>
class H012803gTank;
class H012803gSteering
{
public:
	H012803gSteering(H012803gTank * directedTank);
	~H012803gSteering();

	Vector2D CalculateCumluativeForce();

	void ToggleSeek() { _seek = true; }
	void ToggleFlee() { _flee = true; }
	void ToggleArrive() { _arrive = true; }
	void TogglePursuit() { _pursuit = true; }
	void ToggleOffsetPursuit() { _offsetPursuit = true; }
	void ToggleEvade() { _evade = true; }
	void ToggleWander() { _wander = true; }
	void ToggleObstacleAvoidance() { _obstacleAvoidance = true; }
	void SetTarget(float x, float y) { _target = Vector2D(x, y); }

	Vector2D GetTarget() { return _target; }
private:
	Vector2D _resultantForce;
	H012803gTank* _pTank;
	Vector2D _target;
	std::vector<BaseTank> _tanksInView;

	bool _seek;
	bool _flee;
	bool _arrive;
	bool _pursuit;
	bool _offsetPursuit;
	bool _evade;
	bool _wander;
	bool _obstacleAvoidance;

	Vector2D Seek(Vector2D target);
	Vector2D Flee(Vector2D target);
	Vector2D Arrive(Vector2D target);
	Vector2D Pursuit(const BaseTank* targetAgent);
	Vector2D OffsetPursuit(const BaseTank* targetAgent);
	Vector2D Evade(const BaseTank* targetAgent);
	Vector2D Wander();
	Vector2D ObstacleAvoidance(); 
};

