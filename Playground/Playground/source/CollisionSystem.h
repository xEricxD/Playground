#pragma once
#include <vector>
#include "CollisionComponent.h"

// TODO - add continious collision detection instead of just discrete

// System will handle all collisions in the world
// This system will handle collision, and resolve them
class CollisionSystem
{
public:
  CollisionSystem();
  ~CollisionSystem();

  // Check for collision in the world
  void UpdateCollisionSystem();

  // Add a new collider to the system (each collider will add itself)
  void AddCollider(CollisionComponent* a_collider) { m_colliders.push_back(a_collider); }
  // Remove a collider from the system
  void RemoveCollider(CollisionComponent* a_collider);
  // Clear all colliders from the system ( colliders will handle their own cleanup)
  void ClearCollisionSystem() { m_colliders.clear(); }

private:
  // Generate all AABB's
  void GenerateAABBs();
  // Rebuild the partitioning tree
  void BuildDynamicAABBTree();
  // Do a broad collision check using bounding boxes on every collider
  void PerformBroadPhaseCollisionCheck();
  // perform a narrow collision check on colliders that are potentially colliding
  void PerformNarrowPhaseCollisionCheck();
  // Perform all collision resolves
  void ResolveCollisions();

  std::vector<CollisionComponent*> m_colliders; // Vector with all active colliders
  std::vector < std::pair<CollisionComponent*, CollisionComponent*> > m_broadPhaseCollision; // store the broad phase results in pairs
  std::vector < std::pair<CollisionComponent*, CollisionComponent*> > m_narrowPhaseCollision; // store the narrow phase results in pairs
};

