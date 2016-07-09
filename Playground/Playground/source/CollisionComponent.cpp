#include "CollisionComponent.h"
#include "Engine.h"
#include <SFML/Graphics.hpp>

CollisionComponent::CollisionComponent()
{
}

CollisionComponent::~CollisionComponent()
{
}

void CollisionComponent::Initialise()
{
  GameobjectComponent::Initialise();

  Engine.GetCollisionSystem().AddCollider(this);
}

void CollisionComponent::Update(float a_dt)
{
  GameobjectComponent::Update(a_dt);

  if (m_drawDebug)
    DrawDebug();
}

void CollisionComponent::ShutDown()
{
  GameobjectComponent::ShutDown();
}

void CollisionComponent::DrawDebug()
{
  // Draw the AABB
  sf::RectangleShape box(sf::Vector2f(m_AABB.max.x - m_AABB.min.x, m_AABB.max.y - m_AABB.min.y));
  box.setOrigin(box.getSize() * 0.5f);
  box.setFillColor(sf::Color::Transparent);
  box.setOutlineThickness(1.f);
  box.setOutlineColor(sf::Color::White);

  // get the view matrix to get the correct draw position
  glm::mat4 viewMatrix = m_transform->GetTransformationMatrix() * glm::inverse(Engine.GetCamera().GetTransform().GetTransformationMatrix());
  glm::vec2 drawPosition(viewMatrix[3]); // get the x and y component from the vec3

  box.setPosition(sf::Vector2f(m_transform->GetPosition().x + drawPosition.x, m_transform->GetPosition().y + drawPosition.y));

  Engine.GetWindow().draw(box);
}
