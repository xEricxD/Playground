#pragma once
#include "PathfinderObject.h"
#include "GridObject.h"
#include "PathfindingStructs.h"
#include <queue>


class AStarPathfinderObject : public PathfinderObject
{
public:
  AStarPathfinderObject();
  virtual ~AStarPathfinderObject() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;
  virtual void ShutDown() override;

private:
  inline void SearchLoop(PathfindingPacket a_packet);
  inline void AddNeighbors(PathfindingNode* a_node);
  inline void RelaxPath(PathfindingNode* a_currentNode, PathfindingNode* a_parentNode, short a_cost);

  std::vector<PathfindingNode*> m_fastStack;
};

