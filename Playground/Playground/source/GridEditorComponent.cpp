#include "GridEditorComponent.h"
#include "GameObject.h"
#include "GridObject.h"
#include "World.h"

GridEditorComponent::GridEditorComponent(bool a_allowEditing) : m_pressed(false), m_grid(nullptr), m_allowEditing(a_allowEditing)
{
}

GridEditorComponent::~GridEditorComponent()
{
}

void GridEditorComponent::Initialise()
{
  GameobjectComponent::Initialise();

  m_componentType = GameobjectComponent::ComponentType::GRIDNAVIGATION;

  m_grid = (GridObject*)GetOwner();
}

void GridEditorComponent::Update(float a_dt)
{
  GameobjectComponent::Update(a_dt);

  if (!m_allowEditing)
    return;

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    if (!m_pressed)
    {
      UpdateGridNode();
      m_pressed = true;
    }
  }
  else if (m_pressed)
    m_pressed = false;
}

void GridEditorComponent::UpdateGridNode()
{
  glm::vec2 mousePos(sf::Mouse::getPosition(World.GetWindow()).x, sf::Mouse::getPosition(World.GetWindow()).y);
  glm::vec2 halfscreen((int)(World.GetWindow().getSize().x * 0.5f), (int)(World.GetWindow().getSize().y * 0.5f));

  // use worlds active camera for transformations
  glm::mat4 viewMatrix = glm::inverse(World.GetCamera().GetTransform().GetTransformationMatrix());
  glm::vec2 drawPosition(viewMatrix[3]); // get the x and y component from the vec3

  Node* n = m_grid->GetNodeByPosition(mousePos - halfscreen - drawPosition);
  if (n)
    m_grid->UpdateNodeAvailability(n);
}
