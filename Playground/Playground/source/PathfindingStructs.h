#pragma once
#include "Includes.h"
#include <vector>

struct Node;
class GameObject;

struct Path
{
  std::vector<glm::vec2> path;
};

struct PathfindingPacket
{
  GameObject* requestingActor;
  Node* startNode;
  Node* goalNode;
};