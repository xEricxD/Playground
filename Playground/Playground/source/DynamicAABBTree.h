#pragma once
#include "Includes.h"
#include "CollisionComponent.h"

// a node for tree branches / leaves
struct Node
{
  Node()
    : Parent(nullptr), left(nullptr), right(nullptr)
  {
  }

  // if the node has no children, it is a leaf node
  bool IsLeaf() const { return left == nullptr; }

  void UpdateAABB()
  {
    if (IsLeaf())
    {
      // make the fat AABB
      aabb.min = collider->GetAABB()->min - margin;
      aabb.max = collider->GetAABB()->max + margin;
    }
    else
    {
      // combine both child AABBs to a single aabb
      aabb = left->aabb;
      aabb.Combine(&right->aabb);
    }
  }

  // parent node
  Node* Parent;
  // for easy access to left / right children
  union
  {
    struct 
    {
      Node* left;
      Node* right;
    };
    Node* children[2];
  };

  // the collider stored in this node
  CollisionComponent* collider;
  // "fat" aabb of the collider
  AABB aabb;

  // margin for the fat aabb
  float margin;
};

class DynamicAABBTree
{
public:
  DynamicAABBTree();
  ~DynamicAABBTree();

private:
  std::vector<Node*> m_nodes;

  Node* m_root;
};

