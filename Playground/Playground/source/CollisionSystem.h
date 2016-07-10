#pragma once
#include <SFML\Graphics.hpp>
#include "CollisionComponent.h"
#include <map>

// TODO - add continious collision detection instead of just discrete
class CircleCollisionComponent;
class BoxCollisionComponent;
class ConvexCollisionComponent;

// System will handle all collisions in the world
// This system will handle collision, and resolve them
class CollisionSystem
{
public:
  CollisionSystem();
  ~CollisionSystem();

  void Initialise();
  void ShutDown();

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

  // build the collision check map
  inline void BuildCollisionMap();
  // Perform simple AABB collision check
  inline bool TestCollisionAABB(AABB* A, AABB* B);

  // Collision checking functions
  bool CheckCircleCircleCollision(CollisionComponent* A, CollisionComponent* B);
  bool CheckBoxCircleCollision(CollisionComponent* A, CollisionComponent* B);
  bool CheckCircleBoxCollision(CollisionComponent* A, CollisionComponent* B);
  bool CheckConvexCircleCollision(CollisionComponent* A, CollisionComponent* B);
  bool CheckCircleConvexCollision(CollisionComponent* A, CollisionComponent* B);

  bool CheckConvexCollisions(CollisionComponent* A, CollisionComponent* B);

  std::vector<CollisionComponent*> m_colliders; // Vector with all active colliders
  std::vector < std::pair<CollisionComponent*, CollisionComponent*> > m_broadPhaseCollision; // store the broad phase results in pairs
  std::vector < std::pair<CollisionComponent*, CollisionComponent*> > m_narrowPhaseCollision; // store the narrow phase results in pairs

  // C-style function pointer that returns void type and takes in 2 colliders arguments
  typedef bool (CollisionSystem::*FunctionPtrType)(CollisionComponent* A, CollisionComponent* B);
  // Map to store function pointers for narrow phase collision checks
  std::map<std::pair<CollisionComponent::ColliderType, CollisionComponent::ColliderType>, FunctionPtrType> m_collisionMap;
  
  // Debug variables
  sf::Clock m_clock;
  sf::Int64 m_AABBGenerationTime;
  sf::Int64 m_treeGenerationTime;
  sf::Int64 m_broadPhaseTime;
  sf::Int64 m_narrowPhaseTime;
  sf::Int64 m_collisionResolveTime;
  sf::Int64 m_numUpdates;
};

