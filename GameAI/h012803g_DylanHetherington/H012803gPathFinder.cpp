#include "H012803gPathFinder.h"

H012803gPathFinder::H012803gPathFinder()
{
	SetEdgeCosts();
}
H012803gPathFinder::~H012803gPathFinder()
{
	_edgeCostList.clear();
	_openList.clear();
	_closedList.clear();
}
std::vector<Vector2D> H012803gPathFinder::GetPath(Vector2D start, Vector2D end)
{
	_openList.clear();
	_closedList.clear();
	std::vector<Vector2D> path;
	Waypoint* nearestToTank = GetNearestWaypoint(_pTank->GetCentralPosition());
	Waypoint* nearestToDestination = GetNearestWaypoint(end);
	if (nearestToTank == nullptr || nearestToDestination == nullptr || nearestToTank == nearestToDestination)
	{
		path.push_back(end);
		return path;
	}
	_openList.push_back(new Node(nearestToTank, nullptr, 0.0));
	Node* currentNode = nullptr;
	while (!_openList.empty)
	{
		for (Node* open : _openList)
		{
			if (currentNode == nullptr || open->cost < currentNode->cost)
			{
				currentNode = open;
			}
			if (currentNode == nearestToDestination)
			{

			}
		}
	}
}
void H012803gPathFinder::SetEdgeCosts()
{
	std::vector<Waypoint*> waypoints = WaypointManager::Instance()->GetAllWaypoints();
	for (Waypoint* w : waypoints)
	{
		std::vector<int> connectedPoints = w->GetConnectedWaypointIDs();
		for (int ID : connectedPoints)
		{
			_edgeCostList.push_back(new EdgeCost(w, waypoints[ID], 1.0f));
		}
		connectedPoints.clear();
	}
}
Waypoint* H012803gPathFinder::GetNearestWaypoint(Vector2D position)
{
	std::vector<Waypoint*> waypoints = WaypointManager::Instance()->GetAllWaypoints();
	float nearest = MaxFloat;
	Waypoint* nearestWaypoint = nullptr;
	for (Waypoint* w : waypoints)
	{
		Vector2D vectorBetween = position - w->GetPosition();
		if (vectorBetween.Length < nearest)
		{
			nearest = vectorBetween.Length;
			nearestWaypoint = w;
		}
	}
	return nearestWaypoint;
}
double H012803gPathFinder::GetCostBetweenWaypoints(Waypoint* from, Waypoint* to)
{
	for (EdgeCost* e : _edgeCostList)
	{
		if (e->pWaypointFrom == from && e->pWaypointTo == to)
		{
			return e->cost;
		}
	}
	return MaxDouble;
}
std::vector<Vector2D> H012803gPathFinder::CreatePath(Node* target, Vector2D end)
{
	std::vector<Vector2D> path;
	std::vector<Vector2D> reversePath;
	reversePath.push_back(end);
	Node* current = target;
	while (current != NULL)
	{
		reversePath.push_back(current->pInternalWaypoint->GetPosition());
		current = current->pParent;
	}
	std::vector<Vector2D>::iterator iter = reversePath.end();
	while (iter != reversePath.begin())
	{
		path.push_back(*(--iter));
	}
	return path;
}
bool H012803gPathFinder::IsInList(std::vector<Node*> checkList, Waypoint* checkPoint)
{
	for (Node* n : checkList)
	{
		if (n->pInternalWaypoint == checkPoint)
		{
			return true;
		}
	}
	return false;
}
double H012803gPathFinder::GetHeuristicCost(Vector2D pos1, Vector2D pos2)
{
	Vector2D heuristicCost = pos1 - pos2;
	return heuristicCost.Length;
}