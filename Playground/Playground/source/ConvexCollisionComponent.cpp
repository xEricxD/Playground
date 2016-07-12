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
  CollisionComponent::GenerateAABB();
  // nothing changed, no need to generate the AABB
  if (m_isSleeping)
    return;

  // get the AABB min and max by projecting the shape on the X and Y axis
  glm::vec2 projX = GetProjection(glm::vec2(1, 0));
  glm::vec2 projY = GetProjection(glm::vec2(0, 1));

  m_AABB.min = glm::vec2(projX.x, projY.x);
  m_AABB.max = glm::vec2(projX.y, projY.y);
}

void ConvexCollisionComponent::UpdateVertices()
{
  CollisionComponent::UpdateVertices();

  if (m_isSleeping)
    return;

  m_worldSpaceVertices.clear();

  for (auto vert : m_localSpaceVertices)
  {
    // convert from local to scaled to model to world space
    glm::mat4 scaled = m_transform->GetScaleMatrix() * m_transform->GetTransformationMatrix();
    glm::mat4 modelMatrix = scaled * m_transform->GetRotationMatrix();
    glm::vec2 worldVert((modelMatrix * glm::vec4(vert.x, vert.y, 0, 1)));

    m_worldSpaceVertices.push_back(worldVert);
  }
}

void ConvexCollisionComponent::AddVertices()
{
  // TODO - generate a simple circle for now
  for (int i = 0; i <= 300; i += 60)
  {
    float x = (float)sin((i) * (PI / 180)) * 40;
    float y = (float)cos((i) * (PI / 180)) * 40;
    m_localSpaceVertices.push_back(glm::vec2(x, y));
  }
}