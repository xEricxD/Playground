#include "CollisionSystem.h"


CollisionSystem::CollisionSystem()
{
}

CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::UpdateCollisionSystem()
{
  // Generate all the AABB's of our colliders
  GenerateAABBs();
  // Start by rebuilding our partitioning tree
  BuildDynamicAABBTree();
  // Then do the broad phase collision tests
  PerformBroadPhaseCollisionCheck();
  // Perform narrow phase
  PerformNarrowPhaseCollisionCheck();
  // Resolve any collisions
  ResolveCollisions();
}

void CollisionSystem::RemoveCollider(CollisionComponent* a_collider)
{
  for (auto it = m_colliders.begin(); it != m_colliders.end(); it++)
  {
    if (*it == a_collider)
      m_colliders.erase(it);
  }
}

void CollisionSystem::GenerateAABBs()
{
  for (CollisionComponent* col : m_colliders)
    col->GenerateAABB();
}

void CollisionSystem::BuildDynamicAABBTree()
{
  // clear tree
  // build tree
}

void CollisionSystem::PerformBroadPhaseCollisionCheck()
{
}

void CollisionSystem::PerformNarrowPhaseCollisionCheck()
{
}

void CollisionSystem::ResolveCollisions()
{
}
