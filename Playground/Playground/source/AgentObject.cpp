#include "AgentObject.h"


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
