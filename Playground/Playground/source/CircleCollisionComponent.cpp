#include "CircleCollisionComponent.h"
#include "GameObject.h"

CircleCollisionComponent::CircleCollisionComponent(float a_radius) : m_radius(a_radius)
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
  CollisionComponent::GenerateAABB();
  // nothing changed, no need to generate the AABB
  if (m_isSleeping)
    return;

  glm::vec2 pos = (glm::vec2)(m_transform->GetPosition() + GetOwner()->GetTransform()->GetPosition());

  m_AABB.min = glm::vec2(pos.x - m_radius, pos.y - m_radius);
  m_AABB.max = glm::vec2(pos.x + m_radius, pos.y + m_radius);
}

void CircleCollisionComponent::AddVertices()
{
  // TODO - should be a perfect shape (only distance check), think of how to implement in the collision system
  for (int i = 0; i <= 360; i += 20) 
  {
    float x = (float)sin(i * (PI / 180)) * m_radius;
    float y = (float)cos(i * (PI / 180)) * m_radius;
    m_vertices.push_back(glm::vec2(x, y));
  }
}
