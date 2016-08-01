#pragma once
#include "GridObject.h"
#include "PathfindingStructs.h"
#include <queue>

class PathfinderObject : public GameObject
{
public:
  PathfinderObject();
  virtual  ~PathfinderObject() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;
  virtual void ShutDown() override;

  void RequestPath(GameObject* a_requester, glm::vec2 a_startPosition, glm::vec2 a_goalPosition);
  void RequestPath(GameObject* a_requester, PathfindingNode* a_startNode, PathfindingNode* a_goalNode);

  GridObject* const GetGridObject() { return m_grid; }

protected:
  void CreatePath(PathfindingNode* a_node, Path* a_path);
  void ClearGrid();

  short GetHeuristic(PathfindingNode* a_node);

  GridObject* m_grid;
  std::queue<PathfindingPacket> m_pathQueue;

  std::vector<PathfindingNode*> m_openList;
  std::vector<PathfindingNode*> m_closedList;

  // debugging clock for path timings
  sf::Clock m_clock;
  sf::Int64 m_pathTime;
  sf::Int64 m_numPathRequests;
};

