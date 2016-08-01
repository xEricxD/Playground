#include "PathfinderObject.h"
#include "Engine.h"
#include "AgentObject.h"


PathfinderObject::PathfinderObject()
{
}


PathfinderObject::~PathfinderObject()
{
}

void PathfinderObject::Initialise()
{
  GameObject::Initialise();
  m_gameObjectType = GameObject::GameObjectType::PATHFINDER;

  std::vector<GameObject*> grid = Engine.GetGameObjectsByType(GameObject::GameObjectType::GRID);
  if (grid.size())
    m_grid = (GridObject*)grid[0];
}

void PathfinderObject::Update(float a_dt)
{
  GameObject::Update(a_dt);

}

void PathfinderObject::ShutDown()
{
  GameObject::ShutDown();
}

void PathfinderObject::RequestPath(GameObject * a_requester, glm::vec2 a_startPosition, glm::vec2 a_goalPosition)
{
  PathfindingNode* start = m_grid->GetNodeByPosition(a_startPosition);
  PathfindingNode* goal = m_grid->GetNodeByPosition(a_goalPosition);
  if (start && start->available && goal && goal->available)
    RequestPath(a_requester, start, goal);
  else
    ((AgentObject*)a_requester)->GetNavigationComponent()->PathRequestFailed();
}

void PathfinderObject::RequestPath(GameObject * a_requester, PathfindingNode * a_startNode, PathfindingNode * a_goalNode)
{
  PathfindingPacket packet;
  packet.requestingActor = a_requester;
  packet.startNode = a_startNode;
  packet.goalNode = a_goalNode;

  m_pathQueue.push(packet);
}

void PathfinderObject::CreatePath(PathfindingNode * a_node, Path * a_path)
{
  // backtrace through parents to find our path points
  a_path->path.push_back(a_node->position);
  if (a_node->parentNode)
    CreatePath(a_node->parentNode, a_path);
}

void PathfinderObject::ClearGrid()
{
  // TODO - only reset nodes that have been affected
  m_grid->ClearGrid();

  // Reset all used nodes
  m_closedList.clear();
  m_openList.clear();
}

short PathfinderObject::GetHeuristic(PathfindingNode * a_node)
{
  PathfindingPacket currentPacket;
  currentPacket = m_pathQueue.front();

  short x = (short)(fabs(a_node->gridIndex.x - currentPacket.goalNode->gridIndex.x));
  short y = (short)(fabs(a_node->gridIndex.y - currentPacket.goalNode->gridIndex.y));

  return (x + y) * 10;
}
