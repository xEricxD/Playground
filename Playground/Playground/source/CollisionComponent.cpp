#include "CollisionComponent.h"
#include "Engine.h"
#include "CollisionSystem.h"
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

  Engine.GetCollisionSystem()->AddCollider(this);
  AddVertices();
  GenerateAABB();
  UpdateVertices();
  GetTransform()->ResetChanged();
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

void CollisionComponent::UpdateVertices()
{
  m_isSleeping = (!m_transform->HasChanged());
}

void CollisionComponent::GenerateAABB()
{
  m_isSleeping = (!m_transform->HasChanged());
}

void CollisionComponent::GenerateConvexHull()
{
}

void CollisionComponent::DrawDebug()
{
  // get the view matrix to get the correct draw position
  glm::mat4 modelMatrix = m_transform->GetTransformationMatrix() * GetOwner()->GetTransform()->GetTransformationMatrix();
  glm::mat4 viewMatrix = modelMatrix * glm::inverse(Engine.GetCamera().GetTransform().GetTransformationMatrix());
  glm::vec2 drawPosition(viewMatrix[3]); // get the x and y component from the vec4

  // Draw the AABB
  sf::RectangleShape box(sf::Vector2f(m_AABB.max.x - m_AABB.min.x, m_AABB.max.y - m_AABB.min.y));
  box.setOrigin(box.getSize() * 0.5f);
  box.setFillColor(sf::Color::Transparent);
  box.setOutlineThickness(1.f);
  box.setOutlineColor(sf::Color::White);
  box.setPosition(sf::Vector2f(drawPosition.x, drawPosition.y));

  Engine.GetWindow().draw(box);

  // Draw the complex collider
  if (m_worldSpaceVertices.size() < 2)
    return;

  for (unsigned int i = 0; i < m_worldSpaceVertices.size() - 1; i++)
  {
    glm::vec2 vertex1 = ConvertVertexToViewSpace(m_worldSpaceVertices[i]);
    glm::vec2 vertex2 = ConvertVertexToViewSpace(m_worldSpaceVertices[i + 1]);

    // draw a line between them
    sf::Vertex line[]
    {
      sf::Vertex(sf::Vector2f(vertex1.x, vertex1.y)),
      sf::Vertex(sf::Vector2f(vertex2.x, vertex2.y))
    };

    Engine.GetWindow().draw(line, 2, sf::Lines);
  }
}

inline glm::vec2 CollisionComponent::ConvertVertexToViewSpace(glm::vec2 a_vertex)
{ 
  // convert from vertex to model to view space
  glm::mat4 vertexMatrix = glm::mat4(); // make an identity matrix
  vertexMatrix[3] = glm::vec4(a_vertex, 0, 1); // insert the vertex position into the matrix

  // convert to world space
  glm::mat4 modelMatrix = m_transform->GetTransformationMatrix() * GetOwner()->GetTransform()->GetTransformationMatrix();
  glm::mat4 worldMatrix = modelMatrix * vertexMatrix;
  // back to screen space
  glm::mat4 viewMatrix = worldMatrix * glm::inverse(Engine.GetCamera().GetTransform().GetTransformationMatrix());

  return glm::vec2(viewMatrix[3]);
}
