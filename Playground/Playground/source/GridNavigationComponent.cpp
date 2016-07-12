#include "GridNavigationComponent.h"
#include "AStarPathfinderObject.h"
#include "Engine.h"

GridNavigationComponent::GridNavigationComponent() : m_activePath(nullptr), m_pathfinder(nullptr), m_wander(false)
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
  std::vector<GameObject*> pathfinder = Engine.GetGameObjectsByType(GameObject::GameObjectType::ASTARPATHFINDER);
  if (!pathfinder.empty())
    m_pathfinder = (AStarPathfinderObject*)pathfinder[0];
}

void GridNavigationComponent::Update(float a_dt)
{
  GameobjectComponent::Update(a_dt);

  if (m_wander)
  {
    if (!m_activePath)
    {
      GridObject* grid = m_pathfinder->GetGridObject();
      if (grid)
        RequestPath(grid->GetRandomPositionOnGrid());
    }
  }

  TraversePath(a_dt);
  DrawDebug();
}

void GridNavigationComponent::TraversePath(float a_dt)
{
  if (!m_activePath)
    return;

  if (m_activePath->path.size() > 0)
  {
    // get our target position
    glm::vec2 position(m_transform->GetTransformationMatrix()[3]);
    glm::vec2 distanceVector = m_activePath->path.back() - position;

    if (distanceVector.x != 0 || distanceVector.y != 0)
    {
      glm::vec3 translation = glm::normalize(glm::vec3(distanceVector.x, distanceVector.y, 0));
      translation *= 50 * a_dt;
      GetOwner()->GetTransform()->Translate(translation);
    }

    // check if we're close enough to pop this node of the path
    if (glm::length(distanceVector) < 5)
    {
      m_activePath->path.pop_back();
      if (!m_activePath->path.size())
      {
        delete m_activePath;
        m_activePath = nullptr;
      }
    }
  }
}

void GridNavigationComponent::RequestPath(glm::vec2 a_goalPosition, bool a_stopCurrentPath)
{
  if (!a_stopCurrentPath && m_activePath)
    return;

  //request a path from the pathfinder
  m_pathfinder->RequestPath(GetOwner(), glm::vec2(GetTransform()->GetPosition()), a_goalPosition);
  // stop the current navigation if there is any
  if (m_activePath)
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

  glm::mat4 viewMatrix = m_transform->GetTransformationMatrix() * glm::inverse(Engine.GetCamera().GetTransform().GetTransformationMatrix());
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

    Engine.GetWindow().draw(line, 2, sf::Lines);
  }
}
