#include "CollisionSystem.h"
#include <iostream>
#include <fstream>
#include <string>

CollisionSystem::CollisionSystem()
{
}

CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::ShutDown()
{
  std::ofstream file;
  file.open("../assets/saved/debug.txt", std::ios::app);

  file << "**** Collision system Debug Information ****\n";
  file << "Number of Updates: " << std::to_string(m_numUpdates) << "\n";
  sf::Uint64 time = m_AABBGenerationTime + m_treeGenerationTime + m_broadPhaseTime + m_narrowPhaseTime + m_collisionResolveTime;
  file << "Total time for all updates: " << std::to_string(time / 1000) << " milliseconds\n";
  file << "Average time per update: " << std::to_string(time / m_numUpdates) << " Micro Seconds\n\n";

  file << "Average time per system: \n";
  file << "Average time for AABB generation: " << std::to_string(m_AABBGenerationTime / m_numUpdates) << " Micro Seconds\n";
  file << "Average time for partitioning tree generation: " << std::to_string(m_treeGenerationTime / m_numUpdates) << " Micro Seconds\n";
  file << "Average time for broad phase collision check: " << std::to_string(m_broadPhaseTime / m_numUpdates) << " Micro Seconds\n";
  file << "Average time for narrow phase collision check: " << std::to_string(m_narrowPhaseTime / m_numUpdates) << " Micro Seconds\n";
  file << "Average time for collision resolves: " << std::to_string(m_collisionResolveTime / m_numUpdates) << " Micro Seconds\n\n";

  file.close();
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

  m_numUpdates++;
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
  m_clock.restart();

  for (CollisionComponent* col : m_colliders)
    col->GenerateAABB();

  m_AABBGenerationTime += m_clock.getElapsedTime().asMicroseconds();
}

void CollisionSystem::BuildDynamicAABBTree()
{
  m_clock.restart();
  // clear tree
  // build tree
  m_treeGenerationTime += m_clock.getElapsedTime().asMicroseconds();
}

void CollisionSystem::PerformBroadPhaseCollisionCheck()
{
  m_clock.restart();

  // TODO - once we have a partitioning tree, replace this N^2 loop
  for (unsigned int i = 0; i < m_colliders.size() - 1; i++)
  {
    for (unsigned int j = i + 1; j < m_colliders.size(); j++)
    {
      AABB* A = m_colliders[i]->GetAABB();
      AABB* B = m_colliders[j]->GetAABB();

      // perform collision check
      if (TestCollisionAABB(A, B))
      {
        // object might be colliding, add the pair to broad phase array for further inspection
        std::pair<CollisionComponent*, CollisionComponent*> pair(m_colliders[i], m_colliders[j]);
        m_broadPhaseCollision.push_back(pair);
      }
    }
  }

  m_broadPhaseTime += m_clock.getElapsedTime().asMicroseconds();
}

void CollisionSystem::PerformNarrowPhaseCollisionCheck()
{
  m_clock.restart();

  for (auto collision : m_broadPhaseCollision)
  {

  }

  m_broadPhaseCollision.clear();

  m_narrowPhaseTime += m_clock.getElapsedTime().asMicroseconds();
}

void CollisionSystem::ResolveCollisions()
{
  m_clock.restart();


  m_collisionResolveTime += m_clock.getElapsedTime().asMicroseconds();
}

inline bool CollisionSystem::TestCollisionAABB(AABB* A, AABB* B)
{
  // do simple "box collision" check using the objects AABB
  if ((B->min.x - A->max.x) > 0.0f || (B->min.y - A->max.y) > 0.0f)
    return false;

  if ((A->min.x - B->max.x) > 0.0f || (A->min.y - B->max.y) > 0.0f)
    return false;

  return true;
}
