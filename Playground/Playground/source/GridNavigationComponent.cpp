#include "GridNavigationComponent.h"
#include "AStarPathfinderObject.h"
#include "World.h"

GridNavigationComponent::GridNavigationComponent() : m_activePath(nullptr), m_pathfinder(nullptr)
{
}

GridNavigationComponent::~GridNavigationComponent()
{
}

void GridNavigationComponent::Initialise()
{
  GameobjectComponent::Initialise();

  m_componentType = GameobjectComponent::ComponentType::GRIDNAVIGATION;

  // find the pathfinder in our active scene
  std::vector<GameObject*> pathfinder = World.GetGameObjectsByType(GameObject::GameObjectType::ASTARPATHFINDER);
  if (!pathfinder.empty())
    m_pathfinder = (AStarPathfinderObject*)pathfinder[0];
}

void GridNavigationComponent::Update(float a_dt)
{
  GameobjectComponent::Update(a_dt);

  TraversePath(a_dt);
  DrawDebug();
}

void GridNavigationComponent::TraversePath(float a_dt)
{
  if (!m_activePath)
    return;

  if (m_activePath->path.size() > 0)
  {
    // traverse the path
  }
  else // reset the path to null
  {
    delete m_activePath;
    m_activePath = nullptr;
  }
}

void GridNavigationComponent::RequestPath(glm::vec2 a_goalPosition, bool a_stopCurrentPath)
{
  //request a path from the pathfinder
  m_pathfinder->RequestPath(GetOwner(), glm::vec2(GetOwner()->GetTransform()->GetPosition()), a_goalPosition);
  // stop the current navigation if there is any
  if (a_stopCurrentPath && m_activePath)
  {
    delete m_activePath;
    m_activePath = nullptr;
  }
}

void GridNavigationComponent::PathRequestFailed()
{
  // TODO - change states once states have been implemented
}

void GridNavigationComponent::SetPath(Path * a_path)
{
  if (m_activePath)
    delete m_activePath;

  m_activePath = a_path;
  // TODO - change states
}

void GridNavigationComponent::DrawDebug()
{
  if (!m_drawDebug || !m_activePath)
    return;

  glm::mat4 viewMatrix = m_transform->GetTransformationMatrix() * glm::inverse(World.GetCamera().GetTransform().GetTransformationMatrix());
  glm::vec2 drawPosition(viewMatrix[3]); // get the x and y component from the vec3

  for (unsigned int i = 0; i < m_activePath->path.size() - 1; i++)
  {
    glm::vec2 pos1 = m_activePath->path[i] + drawPosition;
    glm::vec2 pos2 = m_activePath->path[i + 1] + drawPosition;

    sf::Vertex line[]
    {
      sf::Vertex(sf::Vector2f(pos1.x, pos1.y)),
      sf::Vertex(sf::Vector2f(pos2.x, pos2.y))
    };

    World.GetWindow().draw(line, 2, sf::Lines);
  }
}