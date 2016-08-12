#include "JPSPathfinderObject.h"
#include "Engine.h"
#include "AgentObject.h"
#include <iostream>
#include <fstream>
#include <math.h>

//references
//http://zerowidth.com/2013/05/05/jump-point-search-explained.html
//http://grastien.net/ban/articles/hg-aaai11.pdf
//http://gdcvault.com/play/1022094/JPS-Over-100x-Faster-than

//#define DEBUG_STEP_BASED

JPSPathfinderObject::JPSPathfinderObject() : m_updateStepBased(false), m_debuggingPath(false), m_spaceDown(false)
{
  // for debugging path step by step
#ifdef DEBUG_STEP_BASED
  m_updateStepBased = true;
#endif // DEBUG_STEP_BASED
}

JPSPathfinderObject::~JPSPathfinderObject()
{
}

void JPSPathfinderObject::Initialise()
{
  PathfinderObject::Initialise();

}

void JPSPathfinderObject::Update(float a_dt)
{
  PathfinderObject::Update(a_dt);

#ifdef DEBUG_STEP_BASED
  DrawDebug();
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
  {
    if (!m_spaceDown)
    {
      m_spaceDown = true;
      m_debugStep = true;
    }
  }
  else
    m_spaceDown = false;
#endif // DEBUG_STEP_BASED

  while (!m_pathQueue.empty()) // as long as the path queue isn't empty, there is still agents waiting for a path
  {
    m_clock.restart();

    PathfindingPacket currentPacket = m_pathQueue.front();

#ifdef DEBUG_STEP_BASED
    if (!m_debuggingPath)
    {
#endif
      m_debuggingPath = true;

      currentPacket.startNode->parentNode = nullptr;
      m_openList.push_back(currentPacket.startNode);
      currentPacket.startNode->listStatus = ListStatus::ON_OPEN;

#ifdef DEBUG_STEP_BASED
    }
#endif

    SearchLoop(currentPacket);

    m_pathTime += m_clock.getElapsedTime().asMicroseconds();
    m_numPathRequests++;

#ifdef DEBUG_STEP_BASED
    break;
#endif
  }
}

void JPSPathfinderObject::ShutDown()
{
  PathfinderObject::ShutDown();

  std::ofstream file;
  file.open("../assets/saved/debug.txt", std::ios::app);

  file << "**** Jump Point Search Debug Information ****\n";
  file << "Number of path requests: " << std::to_string(m_numPathRequests) << "\n";
  file << "Total time for all paths: " << std::to_string(m_pathTime / 1000) << " milliseconds\n";

  sf::Uint64 averagePathTime = (m_numPathRequests > 0) ? m_pathTime / m_numPathRequests : 0;
  file << "Average time per path: " << std::to_string(averagePathTime) << " Micro Seconds\n\n";

  file.close();
}

void JPSPathfinderObject::SearchLoop(PathfindingPacket &a_packet)
{
  bool goalFound = false;
  while (!goalFound)
  {

#ifdef DEBUG_STEP_BASED
    if (m_updateStepBased && !m_debugStep)
      break;
#endif

    if (m_openList.size() == 0)
    {
      // no valid path can be found
      m_pathQueue.pop(); // remove the requester from the queue since path is invalid


#ifdef DEBUG_STEP_BASED
      m_debuggingPath = false;
#endif

      ((AgentObject*)a_packet.requestingActor)->GetNavigationComponent()->PathRequestFailed();
      return;
    }

    auto lowestNode = m_openList.begin();

    // get the node with the lowest f value of the open list
    for (auto it = m_openList.begin(); it != m_openList.end(); it++)
    {
      if ((*lowestNode)->f > (*it)->f)
        lowestNode = it;
    }

    // remove lowest f node from the open list
    PathfindingNode* node = *lowestNode;
    m_openList.erase(lowestNode);

    if (node != a_packet.goalNode)
    {
      AddNeighbors(node);

      m_closedList.push_back(node);
      node->listStatus = ListStatus::ON_CLOSED;
    }
    else
      goalFound = true;

#ifdef DEBUG_STEP_BASED
    m_debugStep = false;
#endif
  }

  if (goalFound)
  {
    // we found our goal
    Path* path = new Path();
    CreatePath(a_packet.goalNode, path);
    ((AgentObject*)a_packet.requestingActor)->GetNavigationComponent()->SetPath(path);
    m_pathQueue.pop();
    ClearGrid();

#ifdef DEBUG_STEP_BASED
    m_debuggingPath = false;
#endif
  }
}

void JPSPathfinderObject::AddNeighbors(PathfindingNode* a_node)
{  
  //start off by pruning the neighbors (removing any neighbors that do not need examining)
  std::vector<PathfindingNode*> neighbors;
  if (a_node->parentNode)
  {
    glm::vec2 dir = a_node->gridIndex - a_node->parentNode->gridIndex;
    dir.x = clamp(dir.x, -1, 1);
    dir.y = clamp(dir.y, -1, 1);

    // test if we're jumping diagonal or straight
    neighbors = (dir.x != 0 && dir.y != 0) ? JumpDiagonal(a_node, dir) : JumpStraight(a_node, dir);
  }
  else //no parent cell -> dont prune any neighbors
  {
    for (PathfindingNode* neighbor : a_node->neighbors)
      if (neighbor && neighbor->available) //make sure neighbor is available
      {
        neighbor->parentNode = a_node;
        neighbors.push_back(neighbor);
      }
  }

  // Now that we pruned all the neighbors we don't need, try to jump for each neighbor we do need to check
  for (PathfindingNode* neighbor : neighbors)
  {
    glm::vec2 dir = neighbor->gridIndex - a_node->gridIndex;
    dir.x = clamp(dir.x, -1, 1);
    dir.y = clamp(dir.y, -1, 1);

    PathfindingPacket current = m_pathQueue.front();
    PathfindingNode* jumpPoint = FindJumpPoint(a_node, dir, current.startNode, current.goalNode);
    if (jumpPoint && jumpPoint->listStatus == ListStatus::NONE)
    {
      //if we find a jump point, give it all its values and add it to the open list, as with Astar
      jumpPoint->g = a_node->g + GetGValueJPS(jumpPoint, a_node);
      jumpPoint->listStatus = ListStatus::ON_OPEN;
      jumpPoint->parentNode = neighbor->parentNode;
      jumpPoint->h = GetHeuristic(jumpPoint);
      jumpPoint->f = jumpPoint->g + jumpPoint->h;
      m_openList.push_back(jumpPoint);
    }
  }
}

short JPSPathfinderObject::GetGValueJPS(PathfindingNode* a_jumpPoint, PathfindingNode* a_parent)
{
  glm::vec2 dir = a_jumpPoint->gridIndex - a_parent->gridIndex;
  if (dir.x != 0 && dir.y != 0)
    return (short)abs(((a_jumpPoint->gridIndex.x - a_parent->gridIndex.x)) * 14);
  else
  {
    if (dir.x != 0)
      return (short)abs(((a_jumpPoint->gridIndex.x - a_parent->gridIndex.x)) * 10);
    else
      return (short)abs(((a_jumpPoint->gridIndex.y - a_parent->gridIndex.y)) * 10);
  }
}

PathfindingNode * JPSPathfinderObject::FindJumpPoint(PathfindingNode* a_node, glm::vec2 a_dir, PathfindingNode* a_startNode, PathfindingNode* a_targetNode)
{  
  // start of by "jumping" one step in the desired direction
  PathfindingNode* n = m_grid->GetNodeByIndex(a_node->gridIndex + a_dir);
  if (!n || !n->available) // stop jumping if we hit a block, and return null. This jump is invalid and we don't need to look at it any further
    return nullptr;
  if (n == a_targetNode) // we've reached our target, stop jumping and return n
    return n;
  // check if there is any forced neighbors on the node, if so, return n
  if (a_dir.x == 0 || a_dir.y == 0) //straight move
  {
    glm::vec2 offset[2];
    if (a_dir.x != 0) // moving horizontally
      offset[0] = glm::vec2(0, 1), offset[1] = glm::vec2(0, -1);
    else // moving vertically
      offset[0] = glm::vec2(1, 0), offset[1] = glm::vec2(-1, 0);
    for (int i = 0; i < 2; i++)
    {
      if (CheckStraightForcedNeighbor(n, offset[i], a_dir))
        return n; // if a forced neighbor is found, return n so it can be furter examined in the next iteration
    }
  }
  else // diagonal move
  {
    // first check if a diagonal forced neighbor is found, if so, return n
    if (CheckDiagonalForcedNeighbor(n, glm::vec2(-a_dir.x, 0), a_dir))
      return n;
    if (CheckDiagonalForcedNeighbor(n, glm::vec2(0, -a_dir.y), a_dir))
      return n;

    // if not returned at this point, check both straights of the diagonal for a jump point
    // if we find one, return n and end the jump
    if (FindJumpPoint(n, glm::vec2(a_dir.x, 0), a_startNode, a_targetNode))
      return n;
    if (FindJumpPoint(n, glm::vec2(0, a_dir.y), a_startNode, a_targetNode))
      return n;
  }
  // keep recursing until a return is hit
  return FindJumpPoint(n, a_dir, a_startNode, a_targetNode);
}

bool JPSPathfinderObject::CheckStraightForcedNeighbor(PathfindingNode* a_node, glm::vec2 a_offset, glm::vec2 a_dir)
{
  PathfindingNode* n = m_grid->GetNodeByIndex(a_node->gridIndex + a_offset);
  if (!n || !n->available) // test if the node diagonal from our starting node is avaiable
  {
    n = m_grid->GetNodeByIndex(a_node->gridIndex + a_dir + a_offset);
    if (n && n->available) // if it is->
      return true; // c is a forced neighbor, return true
  }

  return false;
}

bool JPSPathfinderObject::CheckDiagonalForcedNeighbor(PathfindingNode* a_node, glm::vec2 a_offset, glm::vec2 a_dir)
{
  PathfindingNode* n = m_grid->GetNodeByIndex(a_node->gridIndex + a_offset);
  if (!n || !n->available) // test if the node diagonal from our starting node is avaiable
  {
    glm::vec2 offset = (a_offset.x == 0) ? glm::vec2(a_dir.x, 0) : glm::vec2(0, a_dir.y);
    n = m_grid->GetNodeByIndex(a_node->gridIndex + offset + a_offset);
    if (n && n->available) // if it is->
      return true; // c is forced, return true
  }
  return false;
}

std::vector<PathfindingNode*> JPSPathfinderObject::JumpStraight(PathfindingNode* a_current, glm::vec2 a_jumpDirection)
{
  std::vector<PathfindingNode*> neighbors;

  // make sure we can move to the node
  PathfindingNode* n = m_grid->GetNodeByIndex(a_current->gridIndex + a_jumpDirection);
  if (!n || !n->available)
    return neighbors;
  // add the node we're trying to move to
  n->parentNode = a_current;
  neighbors.push_back(n);

  // set the correct offsets based on the direction we're trying to move in
  glm::vec2 offset[2];
  if (a_jumpDirection.x != 0) // moving horizontally
  {
    offset[0] = glm::vec2(0, 1);
    offset[1] = glm::vec2(0, -1);
  }
  else // moving vertically
  {
    offset[0] = glm::vec2(1, 0);
    offset[1] = glm::vec2(-1, 0);
  }

  // now check these offets on forced neighbors
  for (int i = 0; i < 2; i++)
  {
    if (CheckStraightForcedNeighbor(a_current, offset[i], a_jumpDirection))
    {
      // forced neighbor check makes sure the cell exists, no need for null checking
      PathfindingNode* forcedNode = m_grid->GetNodeByIndex(a_current->gridIndex + a_jumpDirection + offset[i]);
      forcedNode->parentNode = a_current;
      neighbors.push_back(forcedNode); // if true, the neighbor is forced and we add it to the list of neighbors to check
    }
  }

  return neighbors;
}

std::vector<PathfindingNode*> JPSPathfinderObject::JumpDiagonal(PathfindingNode* a_current, glm::vec2 a_jumpDirection)
{
  std::vector<PathfindingNode*> neighbors;
  //check if we can even move diagonal (if n1 && n2 !walkable, it means we cannot move diagonal , movement would be blocked)
  PathfindingNode* n1 = m_grid->GetNodeByIndex(a_current->gridIndex + glm::vec2(a_jumpDirection.x, 0));
  PathfindingNode* n2 = m_grid->GetNodeByIndex(a_current->gridIndex + glm::vec2(0, a_jumpDirection.y));

  //check if straight cells in the diagonal direction (dir.x and dir.y) are available, if atleast one of them is, we can move diagonal
  if (n1 && n1->available)
    n1->parentNode = a_current, neighbors.push_back(n1);
  if (n2 && n2->available)
    n2->parentNode = a_current, neighbors.push_back(n2);

  //only add the diagonal if atleast one of the other neighbors is added, otherwise they would block the diagonal movement
  if (neighbors.size() > 0)
  {
    PathfindingNode* n = m_grid->GetNodeByIndex(a_current->gridIndex + a_jumpDirection);
    if (n && n->available)
    {
      n->parentNode = a_current;
      neighbors.push_back(n);
    }
  }

  //now check for forced neighbors
  glm::vec2 offset(-a_jumpDirection.x, 0);
  if (CheckDiagonalForcedNeighbor(a_current, offset, a_jumpDirection))
  {
    PathfindingNode* n = m_grid->GetNodeByIndex(a_current->gridIndex + glm::vec2(0, a_jumpDirection.y) + offset);
    n->parentNode = a_current;
    neighbors.push_back(n);
  }

  offset = glm::vec2(0, -a_jumpDirection.y);
  if (CheckDiagonalForcedNeighbor(a_current, offset, a_jumpDirection))
  {
    PathfindingNode* n = m_grid->GetNodeByIndex(a_current->gridIndex + glm::vec2(a_jumpDirection.x, 0) + offset);
    n->parentNode = a_current;
    neighbors.push_back(n);
  }

  return neighbors;
}

void JPSPathfinderObject::DrawDebug()
{
  if (!m_updateStepBased)
    return;

  sf::CircleShape circle;
  circle.setRadius(5);

  glm::mat4 viewMatrix = glm::inverse(Engine.GetActiveCamera().GetTransform().GetTransformationMatrix());
  glm::vec2 drawPosition(viewMatrix[3]); // get the x and y component from the vec3

  if (m_pathQueue.size())
  {
    PathfindingPacket current;
    current = m_pathQueue.front();

    circle.setFillColor(sf::Color::Yellow);
    glm::vec2 pos = drawPosition + current.goalNode->position;
    circle.setPosition(sf::Vector2f(pos.x - 5, pos.y - 5));
    Engine.GetWindow().draw(circle);

    pos = drawPosition + current.startNode->position;
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(sf::Vector2f(pos.x - 5, pos.y - 5));
    Engine.GetWindow().draw(circle);
  }

  for (PathfindingNode* n : m_openList)
  {
    glm::vec2 pos = drawPosition + n->position;
    circle.setFillColor(sf::Color::Blue);
    circle.setPosition(sf::Vector2f(pos.x - 5, pos.y - 5));
    Engine.GetWindow().draw(circle);

    if (n->parentNode)
    {
      glm::vec2 pos2 = drawPosition + n->parentNode->position;
      circle.setFillColor(sf::Color::Blue);
      circle.setPosition(sf::Vector2f(pos2.x - 5, pos2.y - 5));
      Engine.GetWindow().draw(circle);

      sf::Vertex line[]
      {
        sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Blue),
        sf::Vertex(sf::Vector2f(pos2.x, pos2.y), sf::Color::Blue)
      };

      Engine.GetWindow().draw(line, 2, sf::Lines);
    }
  }

  for (PathfindingNode* n : m_closedList)
  {
    glm::vec2 pos = drawPosition + n->position;
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(sf::Vector2f(pos.x - 5, pos.y - 5));
    Engine.GetWindow().draw(circle);

    if (n->parentNode)
    {
      glm::vec2 pos2 = drawPosition + n->parentNode->position;
      circle.setFillColor(sf::Color::Red);
      circle.setPosition(sf::Vector2f(pos2.x - 5, pos2.y - 5));
      Engine.GetWindow().draw(circle);

      sf::Vertex line[]
      {
        sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::Red),
        sf::Vertex(sf::Vector2f(pos2.x, pos2.y), sf::Color::Red)
      };

      Engine.GetWindow().draw(line, 2, sf::Lines);
    }
  }
}
