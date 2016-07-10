#include "ConvexCollisionComponent.h"
#include "GameObject.h"

ConvexCollisionComponent::ConvexCollisionComponent()
{
}

ConvexCollisionComponent::~ConvexCollisionComponent()
{
}

void ConvexCollisionComponent::Initialise()
{
  CollisionComponent::Initialise();

  m_type = ColliderType::CONVEX;
}

void ConvexCollisionComponent::Update(float a_dt)
{
  CollisionComponent::Update(a_dt);
}

void ConvexCollisionComponent::ShutDown()
{
  CollisionComponent::ShutDown();

}

void ConvexCollisionComponent::GenerateAABB()
{
  // TODO - using circle AABB, update this to conex shape!
  CollisionComponent::GenerateAABB();
  // nothing changed, no need to generate the AABB
  if (m_isSleeping)
    return;

  glm::vec2 pos = (glm::vec2)(m_transform->GetPosition());

  m_AABB.min = glm::vec2(pos.x - 20, pos.y - 20);
  m_AABB.max = glm::vec2(pos.x + 20, pos.y + 20);
}

void ConvexCollisionComponent::AddVertices()
{
  // TODO - generate a simple circle for now
  for (int i = 0; i <= 360; i += 20)
  {
    float x = (float)sin(i * (PI / 180)) * 20;
    float y = (float)cos(i * (PI / 180)) * 20;
    m_vertices.push_back(glm::vec2(x, y));
  }
}