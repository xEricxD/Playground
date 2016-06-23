#include "AgentObject.h"
#include "SpriteComponent.h"

// TODO - add better support for sprite changing / updating etc

AgentObject::AgentObject()
{
  // initialise the agents navigation component
  m_navigationComponent = new GridNavigationComponent();
  AddComponent(m_navigationComponent);
}

AgentObject::~AgentObject()
{
}

void AgentObject::Initialise()
{
  GameObject::Initialise();

  m_gameObjectType = GameObject::GameObjectType::AGENT;

  m_spriteComponent = new SpriteComponent("../assets/textures/wizard.png");
  AddComponent(m_spriteComponent);
}

void AgentObject::Update(float a_dt)
{
  GameObject::Update(a_dt);

}

void AgentObject::MoveToLocation(glm::vec2 a_goalLocation)
{
  if (m_navigationComponent)
    m_navigationComponent->RequestPath(a_goalLocation);
}
