#include "AStarPathfinderObject.h"
#include "World.h"
#include "AgentObject.h"

AStarPathfinderObject::AStarPathfinderObject()
{
}

AStarPathfinderObject::~AStarPathfinderObject()
{
}

void AStarPathfinderObject::Initialise()
{
  GameObject::Initialise();

  m_gameObjectType = GameObject::GameObjectType::ASTARPATHFINDER;

  std::vector<GameObject*> grid = World.GetGameObjectsByType(GameObject::GameObjectType::GRID);
  if (grid.size())
    m_grid = (GridObject*)grid[0];
}

void AStarPathfinderObject::RequestPath(GameObject * a_requester, glm::vec2 a_startPosition, glm::vec2 a_goalPosition)
{
  Node* start = m_grid->GetNodeByPosition(a_startPosition);
  Node* goal = m_grid->GetNodeByPosition(a_goalPosition);
  if (start && goal)
    RequestPath(a_requester, start, goal);
  else
    ((AgentObject*)a_requester)->GetNavigationComponent()->PathRequestFailed();
}

void AStarPathfinderObject::RequestPath(GameObject * a_requester, Node * a_startNode, Node * a_goalNode)
{
  PathfindingPacket packet;
  packet.requestingActor = a_requester;
  packet.startNode = a_startNode;
  packet.goalNode = a_goalNode;

  m_pathQueue.push(packet);
}

void AStarPathfinderObject::Update(float a_dt)
{
  GameObject::Update(a_dt);

  while (!m_pathQueue.empty()) // as long as the path queue isn't empty, there is still agents waiting for a path
  {
    ClearGrid();
    PathfindingPacket currentPacket = m_pathQueue.front();
    currentPacket.startNode->parentNode = nullptr;
    m_openList.push_back(currentPacket.startNode);
    currentPacket.startNode->listStatus = ListStatus::ON_OPEN;

    SearchLoop(currentPacket);
  }
}

void AStarPathfinderObject::SearchLoop(PathfindingPacket a_packet)
{
  bool goalReached = false;
  while (!goalReached)
  {
    if (!m_openList.size())
    {
      // no more nodes on open list, we did not find a path to our goal
      m_pathQueue.pop();
      ((AgentObject*)a_packet.requestingActor)->GetNavigationComponent()->PathRequestFailed();
      return;
    }

    Node* node = m_openList[0];

    // TODO -> implement fast stack

    // get node with lowest F value from the open list
    for (Node* n : m_openList)
    {
      if (node->f > n->f)
        node = n;
    }

    // remove lowest f node form open list
    for (auto it = m_openList.begin(); it != m_openList.end(); it++)
    {
      if (*it == node)
      {
        m_openList.erase(it);
        break;
      }
    }

    if (node != a_packet.goalNode)
    {
      AddNeighbors(node);

      m_closedList.push_back(node);
      node->listStatus = ListStatus::ON_CLOSED;
    }
    else
      goalReached = true;
  }

  // we found our goal
  Path* path = new Path();
  CreatePath(a_packet.goalNode, path);
  ((AgentObject*)a_packet.requestingActor)->GetNavigationComponent()->SetPath(path);
  m_pathQueue.pop();
}

void AStarPathfinderObject::AddNeighbors(Node * a_node)
{
  for (int i = 0; i < 8; i++) // loop over all neighbors
  {
    if (a_node->neighbors[i])
    {
      Node* neighbor = a_node->neighbors[i];
      if (neighbor->listStatus != ListStatus::ON_CLOSED)
      {
        short g = (i < 4) ? 10 : 14; // we know that in the grid the first 4 neighbors are straight moves, since that's how we assign them
        if (neighbor->listStatus != ListStatus::ON_OPEN)
        {
          neighbor->listStatus = ListStatus::ON_OPEN;
          neighbor->parentNode = a_node;
          neighbor->g = a_node->g + g;
          neighbor->h = GetHueristic(neighbor);
          neighbor->f = neighbor->h + neighbor->g;

          m_openList.push_back(neighbor);
        }
        else // check if the path through the current node is shorter than the path already assigned to the neighbor
          RelaxPath(neighbor, a_node, g);
      }
    }
  }
}

void AStarPathfinderObject::RelaxPath(Node * a_currentNode, Node * a_parentNode, short a_cost)
{
  // if true, moving through the parent node to the current node is shorter than the path currently assigned to the current node
  if (a_currentNode->g > a_parentNode->g + a_cost)
  {
    a_currentNode->parentNode = a_parentNode;
    a_currentNode->g = a_parentNode->g + a_cost;
    // no need to recalculate H cost, it will remain the same and has already been calculated since the node is on the open list
    a_currentNode->f = a_currentNode->h + a_currentNode->g;
  }
}

short AStarPathfinderObject::GetHueristic(Node * a_node)
{
  PathfindingPacket packet = m_pathQueue.front();

  short x = (short)abs(a_node->gridIndex.x - packet.goalNode->gridIndex.x);
  short y = (short)abs(a_node->gridIndex.y - packet.goalNode->gridIndex.y);

  return (x + y) * 10;
}

void AStarPathfinderObject::CreatePath(Node* a_node, Path* a_path)
{
  // backtrace through parents to find our path points
  a_path->path.push_back(a_node->position);
  if (a_node->parentNode)
    CreatePath(a_node->parentNode, a_path);
}

void AStarPathfinderObject::ClearGrid()
{ 
  // TODO - only reset nodes that have been affected
  m_grid->ClearGrid();

  //Reset all used nodes
  m_closedList.clear();
  m_openList.clear();
}
