#include "CircleCollisionComponent.h"
#include "GameObject.h"
#include "Engine.h"

CircleCollisionComponent::CircleCollisionComponent(float a_radius) : m_radius(a_radius)
{
}

CircleCollisionComponent::~CircleCollisionComponent()
{
}

void CircleCollisionComponent::Initialise()
{
  CollisionComponent::Initialise();

  m_type = ColliderType::CIRCLE;
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

  glm::vec2 pos = (glm::vec2)(m_transform->GetPosition());

  m_AABB.min = glm::vec2(pos.x - m_radius, pos.y - m_radius);
  m_AABB.max = glm::vec2(pos.x + m_radius, pos.y + m_radius);
}

void CircleCollisionComponent::DrawDebug()
{
  CollisionComponent::DrawDebug();

  // get the view matrix to get the correct draw position
  glm::mat4 modelMatrix = m_transform->GetTransformationMatrix() * GetOwner()->GetTransform()->GetTransformationMatrix();
  glm::mat4 viewMatrix = modelMatrix * glm::inverse(Engine.GetCamera().GetTransform().GetTransformationMatrix());
  glm::vec2 drawPosition(viewMatrix[3]); // get the x and y component from the vec4

   // draw the circle
  sf::CircleShape circle(m_radius);
  circle.setOrigin(sf::Vector2f(m_radius, m_radius));
  circle.setFillColor(sf::Color::Transparent);
  circle.setOutlineThickness(1.f);
  circle.setOutlineColor(sf::Color::White);
  circle.setPosition(sf::Vector2f(drawPosition.x, drawPosition.y));

  Engine.GetWindow().draw(circle);
}
