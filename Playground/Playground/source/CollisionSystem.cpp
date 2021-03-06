#include "CollisionSystem.h"
#include "CircleCollisionComponent.h"
#include "ConvexCollisionComponent.h"
#include <iostream>
#include <fstream>
#include <string>

// Sources: 
// http://www.dyn4j.org/2010/01/sat/#sat-convex
// http://www.sevenson.com.au/actionscript/sat/
// https://www.toptal.com/game/video-game-physics-part-ii-collision-detection-for-solid-objects

CollisionSystem::CollisionSystem() : m_colliderUpdateTime(0), m_AABBGenerationTime(0), m_treeGenerationTime(0)
, m_broadPhaseTime(0), m_narrowPhaseTime(0), m_collisionResolveTime(0), m_numUpdates(0)
{
}

CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::Initialise()
{
  BuildCollisionMap();
}

void CollisionSystem::ShutDown()
{
  std::ofstream file;
  file.open("../assets/saved/debug.txt", std::ios::app);

  file << "**** Collision system Debug Information ****\n";
  file << "Number of Updates: " << std::to_string(m_numUpdates) << "\n";
  sf::Uint64 time = m_colliderUpdateTime + m_AABBGenerationTime + m_treeGenerationTime + m_broadPhaseTime + m_narrowPhaseTime + m_collisionResolveTime;
  file << "Total time for all updates: " << std::to_string(time / 1000) << " milliseconds\n";
  file << "Average time per update: " << std::to_string(time / m_numUpdates) << " Micro Seconds\n\n";

  file << "Average time per system: \n";
  file << "Average time for Collider Updates: " << std::to_string(m_colliderUpdateTime / m_numUpdates) << " Micro Seconds\n";
  file << "Average time for AABB generation: " << std::to_string(m_AABBGenerationTime / m_numUpdates) << " Micro Seconds\n";
  file << "Average time for partitioning tree generation: " << std::to_string(m_treeGenerationTime / m_numUpdates) << " Micro Seconds\n";
  file << "Average time for broad phase collision check: " << std::to_string(m_broadPhaseTime / m_numUpdates) << " Micro Seconds\n";
  file << "Average time for narrow phase collision check: " << std::to_string(m_narrowPhaseTime / m_numUpdates) << " Micro Seconds\n";
  file << "Average time for collision resolves: " << std::to_string(m_collisionResolveTime / m_numUpdates) << " Micro Seconds\n\n";

  file.close();
}

void CollisionSystem::UpdateCollisionSystem()
{
  // Update all colliders
  UpdateColliders();
  // Generate all the AABB's of our colliders
  GenerateAABBs();
  // Start by rebuilding our partitioning tree
  BuildDynamicAABBTree();
  // Reset transform
  ResetTransforms();
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


void CollisionSystem::UpdateColliders()
{
  m_clock.restart();

  for (CollisionComponent* col : m_colliders)
    col->UpdateVertices();

  m_colliderUpdateTime += m_clock.getElapsedTime().asMicroseconds();
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

void CollisionSystem::ResetTransforms()
{
  for (CollisionComponent* col : m_colliders)
    col->GetTransform()->ResetChanged();
}

void CollisionSystem::PerformBroadPhaseCollisionCheck()
{
  m_clock.restart();

  // TODO - once we have a partitioning tree, replace this N^2 loop
  for (unsigned int i = 0; i < m_colliders.size(); i++)
  {
    for (unsigned int j = i + 1; j < m_colliders.size(); j++)
    {
      CollisionComponent* c1 = m_colliders[i];
      CollisionComponent* c2 = m_colliders[j];

      // if both are sleeping, there can't be a new collision
      if (c1->IsSleeping() && c2->IsSleeping())
        continue;

      AABB* A = c1->GetAABB();
      AABB* B = c2->GetAABB();
      // perform collision check
      if (TestCollisionAABB(A, B))
      {
        // object might be colliding, add the pair to broad phase array for further inspection
        m_broadPhaseCollision.push_back(std::pair<CollisionComponent*, CollisionComponent*>(c1, c2));
      }
    }
  }

  m_broadPhaseTime += m_clock.getElapsedTime().asMicroseconds();
}

void CollisionSystem::PerformNarrowPhaseCollisionCheck()
{
  m_clock.restart();

  // use seperating axis theorem to check for collision
  for (auto collision : m_broadPhaseCollision)
  {
    std::pair<CollisionComponent::ColliderType, CollisionComponent::ColliderType> pair(collision.first->GetColliderType(), collision.second->GetColliderType());
    if (m_collisionMap.find(pair) != m_collisionMap.end())
    {
      // this is a special case, so call the corresponding function
      if ((this->*m_collisionMap[pair])(collision.first, collision.second))
        m_narrowPhaseCollision.push_back(collision);
    }
    else
    {
      if (CheckConvexCollisions(collision.first, collision.second))
        m_narrowPhaseCollision.push_back(collision);
    }
  }
  m_broadPhaseCollision.clear();

  m_narrowPhaseTime += m_clock.getElapsedTime().asMicroseconds();
}

void CollisionSystem::ResolveCollisions()
{
  m_clock.restart();

  for (auto collision : m_narrowPhaseCollision)
  {

  }
  m_narrowPhaseCollision.clear();

  m_collisionResolveTime += m_clock.getElapsedTime().asMicroseconds();
}

inline void CollisionSystem::BuildCollisionMap()
{
  std::pair<CollisionComponent::ColliderType, CollisionComponent::ColliderType> pair(CollisionComponent::ColliderType::CIRCLE, CollisionComponent::ColliderType::CIRCLE);
  m_collisionMap[pair] = &CollisionSystem::CheckCircleCircleCollision;

  pair.first = CollisionComponent::ColliderType::CIRCLE, pair.second = CollisionComponent::ColliderType::CONVEX;
  m_collisionMap[pair] = &CollisionSystem::CheckCircleConvexCollision;

  pair.first = CollisionComponent::ColliderType::CONVEX, pair.second = CollisionComponent::ColliderType::CIRCLE;
  m_collisionMap[pair] = &CollisionSystem::CheckConvexCircleCollision;
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

inline std::vector<glm::vec2> CollisionSystem::GetConvexColliderNormals(CollisionComponent * a_collider)
{
  std::vector<glm::vec2> verts = a_collider->GetWorldSpaceVertices();

  std::vector<glm::vec2> normals;

  for (unsigned int i = 0; i < verts.size() - 1; i++)
    normals.push_back(CalculateNormalBetweenTwoVertices(verts[i], verts[i + 1]));

  // and add the axis from verts.last - verts.first
  normals.push_back(CalculateNormalBetweenTwoVertices(verts.back(), verts.front()));

  return normals;
}

inline glm::vec2 CollisionSystem::CalculateNormalBetweenTwoVertices(glm::vec2 a_v1, glm::vec2 a_v2)
{
  // get the vector from p1->p2
  glm::vec2 edge = a_v2 - a_v1;
  // calculate the normal vector
  glm::vec2 normal = glm::normalize(glm::vec2(edge.y, -edge.x));
  return normal;
}

inline bool CollisionSystem::CheckOverlap(glm::vec2 a_projectionA, glm::vec2 a_projectionB)
{
  // get projection with smallest min on the left side of our check
  glm::vec2 p1 = a_projectionA;
  glm::vec2 p2 = a_projectionB;
  if (a_projectionA.x > a_projectionB.x)
  {
    p1 = a_projectionB;
    p2 = a_projectionA;
  }
  
  // check if there is a gap
  // we know p1 is on the left side of the projection, so will always be overlap if p1max is not smaller than p2min
  if (p1.y/*max*/ < p2.x/*min*/)
    return false;

  return true;
}

bool CollisionSystem::CheckCircleCircleCollision(CollisionComponent* A, CollisionComponent* B)
{
  CircleCollisionComponent* c1 = (CircleCollisionComponent*)A;
  CircleCollisionComponent* c2 = (CircleCollisionComponent*)B;

  float dx = c1->GetTransform()->GetPosition().x - c2->GetTransform()->GetPosition().x;
  float dy = c1->GetTransform()->GetPosition().y - c2->GetTransform()->GetPosition().y;
  float distanceSq = dx * dx + dy * dy;
  float radiusSq = (c1->GetRadius() + c2->GetRadius()) *  (c1->GetRadius() + c2->GetRadius());
  // if the distance is shorter the the radii, the circles are colliding
  return distanceSq <= radiusSq;
}

bool CollisionSystem::CheckConvexCircleCollision(CollisionComponent* A, CollisionComponent* B)
{
  return CheckCircleConvexCollision(B, A);
}

bool CollisionSystem::CheckCircleConvexCollision(CollisionComponent* A, CollisionComponent * B)
{
  CircleCollisionComponent* c1 = (CircleCollisionComponent*)A;
  ConvexCollisionComponent* c2 = (ConvexCollisionComponent*)B;
  c1, c2;

  return false;
}

bool CollisionSystem::CheckConvexCollisions(CollisionComponent* A, CollisionComponent* B)
{
  // use the seperating axis theorem to check for collision
  // make sure both colliders have atleast 2 verts
  if (A->GetWorldSpaceVertices().size() <= 1 || B->GetWorldSpaceVertices().size() <= 1)
    return false;

  // Get all the axes we want to project on
  std::vector<glm::vec2> axes = GetConvexColliderNormals(A);
  std::vector<glm::vec2> axes2 = GetConvexColliderNormals(A);
  //insert axes2 into axes so we have a single vector
  axes.insert(axes.end(), axes2.begin(), axes2.end());

  for (auto axis : axes)
  {
    // get the projection of both shapes
    glm::vec2 p1 = A->GetProjection(axis);
    glm::vec2 p2 = B->GetProjection(axis);

    // if the shapes do not overlap on 1 axis, there is no collision
    if (!CheckOverlap(p1, p2))
      return false;
  }

  return true;
}
