#pragma once
#include "GameObject.h"
#include "GridObject.h"
#include "PathfindingStructs.h"
#include <queue>


class AStarPathfinderObject : public GameObject
{
public:
  AStarPathfinderObject();
  virtual ~AStarPathfinderObject() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;
  virtual void ShutDown() override;

  // agents can request this pathfinder for a path via this function
  void RequestPath(GameObject* a_requester, glm::vec2 a_startPosition, glm::vec2 a_goalPosition);
  void RequestPath(GameObject* a_requester, Node* a_startNode, Node* a_goalNode);

  GridObject* GetGridObject() { return m_grid; }

private:
  inline void SearchLoop(PathfindingPacket a_packet);
  inline void AddNeighbors(Node* a_node);
  inline void RelaxPath(Node* a_currentNode, Node* a_parentNode, short a_cost);

  short GetHueristic(Node* a_node);
  void CreatePath(Node* a_node, Path* a_path);
  void ClearGrid();

  GridObject* m_grid;
  std::queue<PathfindingPacket> m_pathQueue;

  std::vector<Node*> m_openList;
  std::vector<Node*> m_fastStack;
  std::vector<Node*> m_closedList;

  //debugging clock for path timings
  sf::Clock m_clock;
  sf::Int64 m_pathTime;
  sf::Int64 m_numPathRequests;
};

