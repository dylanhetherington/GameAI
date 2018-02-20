#include "H012803gSteering.h"

H012803gSteering::H012803gSteering(H012803gTank * directedTank)
{
	_pTank = directedTank;
	_resultantForce = Vector2D(0.0f, 0.0f);
	_seek = false;
	_flee = false;
	_arrive = false;
	_pursuit = false;
	_offsetPursuit = false;
	_evade = false;
	_wander = false;
	_obstacleAvoidance = false;
	_detectionBoxLength = 0.0;
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
	if (_obstacleAvoidance == true)
	{
		force += ObstacleAvoidance(ObstacleManager::Instance()->GetObstacles());
	}
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
		force += Arrive(_target);
	}
	if (_pursuit == true && _targetAgent != nullptr)
	{
		force += Pursuit(_targetAgent);
	}
	return force;
}
Vector2D H012803gSteering::Seek(Vector2D target)
{
	Vector2D velocity = Vec2DNormalize(target - _pTank->GetCentralPosition()) * _pTank->GetMaxSpeed();
	return velocity;
}
Vector2D H012803gSteering::Flee(Vector2D target)
{
	Vector2D velocity = Vec2DNormalize(_pTank->GetPosition() - target)	* _pTank->GetMaxSpeed();
	return  velocity;
}
Vector2D H012803gSteering::Arrive(Vector2D target)
{
	Vector2D ToTarget = target - _pTank->GetCentralPosition();
	Vector2D position = _pTank->GetCentralPosition();
	//calculate the distance to the target
	double distance = ToTarget.Length();

	if (distance > 0)
	{
		const double DecelerationTweaker = 0.3;

		//calculate the speed required to reach the target given the desired
		//deceleration
		double speed = distance / (3.0 * DecelerationTweaker);

		//make sure the velocity does not exceed the max
		speed = min(speed, _pTank->GetMaxSpeed());
		Vector2D velocity = (ToTarget * speed / distance) -_pTank->GetVelocity();

		return velocity;
	}
	return Vector2D(0, 0);
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
Vector2D H012803gSteering::ObstacleAvoidance(std::vector<GameObject*> obstacles)
{
	GameObject* close = nullptr;
	Vector2D heading = _pTank->GetHeading();
	heading.Normalize();
	double closeObjectDistance = MaxDouble;
	Vector2D closeTarget;
	for (int i = 0; i < obstacles.size(); i++)
	{
		Vector2D vectorToTarget = obstacles[i]->GetCentralPosition() - _pTank->GetCentralPosition();
		double vectorToTargetLength = vectorToTarget.Length();

		if (vectorToTargetLength < kFieldOfViewLength)
		{
			vectorToTarget.Normalize();

			if (vectorToTargetLength < closeObjectDistance)
			{
				closeObjectDistance = vectorToTargetLength;
				closeTarget = vectorToTarget;
				close = obstacles[i];
			}
		}
		if (close != nullptr)
		{
			double dotProduct = heading.Dot(closeTarget);
			if (dotProduct > kFieldOfView)
			{
				//Vector2D point1 = _pTank->GetCentralPosition() + 10 * heading;
				//Vector2D point2 = _pTank->GetCentralPosition() + 10/2 * heading;
				//Vector2D point3 = _pTank->GetCentralPosition() + 10/2 * heading;
				Vector2D point1 = _pTank->GetCentralPosition() + (closeTarget*(vectorToTargetLength*0.33f));
				Vector2D point2 = _pTank->GetCentralPosition() + (closeTarget*(vectorToTargetLength*0.5f));
				Vector2D point3 = _pTank->GetCentralPosition() + (closeTarget*(vectorToTargetLength*0.66f));
				std::vector<Vector2D> rect = close->GetAdjustedBoundingBox();

				if ((Collisions::Instance()->TriangleCollision(rect[1], rect[2], rect[3], point1)) || (Collisions::Instance()->TriangleCollision(rect[0], rect[1], rect[3], point1)))
				{
					Vector2D OverShoot = point1 - close->GetCentralPosition();
					Vector2D normalized = close->GetCentralPosition();
					normalized.Normalize();
					return Vector2D(normalized * OverShoot.Length());
				}
				else if ((Collisions::Instance()->TriangleCollision(rect[1], rect[2], rect[3], point2)) || (Collisions::Instance()->TriangleCollision(rect[0], rect[1], rect[3], point2)))
				{
					Vector2D OverShoot = point2 - close->GetCentralPosition();
					Vector2D normalized = close->GetCentralPosition();
					normalized.Normalize();
					return Vector2D(normalized * OverShoot.Length());
				}
				else if ((Collisions::Instance()->TriangleCollision(rect[1], rect[2], rect[3], point3)) || (Collisions::Instance()->TriangleCollision(rect[0], rect[1], rect[3], point3)))
				{
					Vector2D OverShoot = point3 - close->GetCentralPosition();
					Vector2D normalized = close->GetCentralPosition();
					normalized.Normalize();
					return Vector2D(normalized * OverShoot.Length());
				}
			}
		}
	}
	return Vector2D(0, 0);
}
/*Vector2D H012803gSteering::ObstacleAvoidance(std::vector<GameObject*> obstacles)
{
	_detectionBoxLength = 1.0 + (_pTank->GetCurrentSpeed() / _pTank->GetMaxSpeed()) * 1.0;
	Vector2D velocity;
	double distanceToClosest = MaxDouble;
	GameObject* closestObject = nullptr;
	Vector2D closestPosition;
	Vector2D SteeringForce;
	for (int i = 0; i < obstacles.size(); i++)
	{
		Vector2D localPosition = ConvertToLocalSpace(obstacles[i]->GetPosition(),_pTank->GetHeading(), _pTank->GetSide(), _pTank->GetPosition());
		if (localPosition.x >= 0)
		{
			double radius = obstacles[i]->GetCollisionRadius() + _pTank->GetCollisionRadius();

			if (localPosition.y < radius)
			{
				double closestX = localPosition.x;
				double closestY = localPosition.y;
				double SqrtClosest = sqrt(radius * radius - closestY * closestY);
				double ip = closestX - SqrtClosest;
				if (ip <= 0.0)
				{
					ip = closestX + SqrtClosest;
				}
				if (ip < distanceToClosest)
				{
					distanceToClosest = ip;
					closestObject = obstacles[i];
					closestPosition = localPosition;
				}
			}
		}
	}
	if (closestObject)
	{
		double multiplier = 1.0 + (_detectionBoxLength - closestPosition.x) / _detectionBoxLength;
		SteeringForce.y = (closestObject->GetCollisionRadius() - closestPosition.y) * multiplier;
		SteeringForce.x = (closestObject->GetCollisionRadius() - closestPosition.x) * 0.2;
	}
	return VectorToWorldSpace(SteeringForce, _pTank->GetHeading(), _pTank->GetSide());
}*/
