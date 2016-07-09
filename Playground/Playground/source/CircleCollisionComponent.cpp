#include "CircleCollisionComponent.h"

CircleCollisionComponent::CircleCollisionComponent()
{
}

CircleCollisionComponent::~CircleCollisionComponent()
{
}

void CircleCollisionComponent::Initialise()
{
  CollisionComponent::Initialise();
}

void CircleCollisionComponent::Update(float a_dt)
{
  CollisionComponent::Update(a_dt);
}

void CircleCollisionComponent::ShutDown()
{
  CollisionComponent::ShutDown();

}

void CircleCollisionComponent::GenerateAABB()
{
  glm::vec2 pos = (glm::vec2)m_transform->GetPosition();

  m_AABB.min = glm::vec2(pos.x - m_radius, pos.y - m_radius);
  m_AABB.max = glm::vec2(pos.x + m_radius, pos.y + m_radius);
}
