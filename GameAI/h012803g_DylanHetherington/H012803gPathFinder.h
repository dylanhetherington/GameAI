#pragma once
#include "../Waypoint.h"
#include "../WaypointManager.h"
#include "h012803gTank.h"

struct EdgeCost
{
	Waypoint* pWaypointFrom;
	Waypoint* pWaypointTo;
	double cost;
	EdgeCost(Waypoint* from, Waypoint* to, double newCost)
	{
		pWaypointFrom = from;
		pWaypointTo = to;
		cost = newCost;
	}
};
struct Node
{
	Waypoint* pInternalWaypoint;
	Node* pParent;
	double cost;
	Node(Waypoint* current, Node* parent, double newCost)
	{
		pInternalWaypoint = current;
		pParent = parent;
		cost = newCost;
	}
};
class H012803gTank;
class H012803gPathFinder
{
public:
	H012803gPathFinder();
	~H012803gPathFinder();
	std::vector<Vector2D> GetPath(Vector2D start, Vector2D end);
private:
	std::vector<EdgeCost*>	_edgeCostList;
	std::vector<Node*>		_openList;
	std::vector<Node*>		_closedList;

	H012803gTank*			_pTank;

	void					SetEdgeCosts();
	Waypoint*				GetNearestWaypoint(Vector2D position);
	double					GetCostBetweenWaypoints(Waypoint* from, Waypoint* to);
	std::vector<Vector2D>	CreatePath(Node* target, Vector2D end);
	bool					IsInList(std::vector<Node*> checkList, Waypoint* checkPoint);
	double					GetHeuristicCost(Vector2D pos1, Vector2D pos2);
};
