#pragma once
#include "Includes.h"
#include <vector>

struct PathfindingNode;
class GameObject;

struct Path
{
  std::vector<glm::vec2> path;
};

struct PathfindingPacket
{
  GameObject* requestingActor;
  PathfindingNode* startNode;
  PathfindingNode* goalNode;
};