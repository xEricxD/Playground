#include "GameobjectComponent.h"
#include "World.h"

GameobjectComponent::GameobjectComponent() : m_initialised(false), m_transform(nullptr)
{

}

GameobjectComponent::~GameobjectComponent()
{
  if (m_transform)
    delete m_transform;
}

void GameobjectComponent::Initialise()
{
  m_transform = new TransformComponent();

  m_initialised = true;
}

void GameobjectComponent::Update(float a_dt)
{
}
