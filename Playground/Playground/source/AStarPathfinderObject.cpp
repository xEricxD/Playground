#include "AStarPathfinderObject.h"


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
}

void AStarPathfinderObject::RequestPath(GameObject * a_requester, glm::vec2 a_startPosition, glm::vec2 a_goalPosition)
{
  Node* start = m_grid->GetNodeByPosition(a_startPosition);
  Node* goal = m_grid->GetNodeByPosition(a_goalPosition);
  if (start && goal)
    RequestPath(a_requester, start, goal);
  //else
  // TODO -> send requester path fail message
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
      // TODO -> send requester a path fail message
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
  // TODO -> send requester the found path
  m_pathQueue.pop();
  ClearGrid();
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
    CreatePath(a_node, a_path);
}

void AStarPathfinderObject::ClearGrid()
{ 
  //Reset all used nodes
  for (Node* node : m_closedList)
  {
    node->f = node->h = node->g = 0;
    node->listStatus = ListStatus::NONE;
    node->parentNode = nullptr;
  }
  m_closedList.clear();

  for (Node* node : m_openList)
  {
    node->f = node->h = node->g = 0;
    node->listStatus = ListStatus::NONE;
    node->parentNode = nullptr;
  }
  m_openList.clear();
}
