#pragma once
#include "PathfinderObject.h"
#include "GridObject.h"
#include "PathfindingStructs.h"
#include <queue>

// TODO - fix corner cutting

class JPSPathfinderObject : public PathfinderObject
{
public:
  JPSPathfinderObject();
  virtual  ~JPSPathfinderObject() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;
  virtual void ShutDown() override;

private:
  inline void SearchLoop(PathfindingPacket &a_packet);

  void AddNeighbors(PathfindingNode* a_cell);

  short GetGValueJPS(PathfindingNode* a_jumpPoint, PathfindingNode* a_parent);
  PathfindingNode* FindJumpPoint(PathfindingNode* a_cell, glm::vec2 a_dir, PathfindingNode* a_startNode, PathfindingNode* a_targetNode);
  bool CheckStraightForcedNeighbor(PathfindingNode* a_node, glm::vec2 a_offset, glm::vec2 a_dir);
  bool CheckDiagonalForcedNeighbor(PathfindingNode* a_node, glm::vec2 a_offset, glm::vec2 a_dir);

  std::vector<PathfindingNode*> JumpStraight(PathfindingNode* a_current, glm::vec2 a_jumpDirection);
  std::vector<PathfindingNode*> JumpDiagonal(PathfindingNode* a_current, glm::vec2 a_jumpDirection);
  
  // debugging variables for path checking
  void DrawDebug();
  bool m_debugStep;
  bool m_updateStepBased;
  bool m_debugdraw;
  bool m_debuggingPath;
  bool m_spaceDown;

  // debugging clock for path timings
  sf::Clock m_clock;
  sf::Int64 m_pathTime;
  sf::Int64 m_numPathRequests;
};

