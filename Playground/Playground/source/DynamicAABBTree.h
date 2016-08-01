#pragma once
#include "Includes.h"
#include "CollisionComponent.h"

// a PathfindingNode for tree branches / leaves
struct TreeNode
{
  TreeNode()
    : Parent(nullptr), left(nullptr), right(nullptr)
  {
  }

  // if the PathfindingNode has no children, it is a leaf PathfindingNode
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

  // parent PathfindingNode
  TreeNode* Parent;
  // for easy access to left / right children
  union
  {
    struct 
    {
      TreeNode* left;
      TreeNode* right;
    };
    TreeNode* children[2];
  };

  // the collider stored in this PathfindingNode
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
  std::vector<TreeNode*> m_nodes;

  TreeNode* m_root;
};

