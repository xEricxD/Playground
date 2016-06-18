#include "GameObject.h"
#include "Camera.h"
#include "World.h"


GameObject::GameObject() : m_initialised(false), m_transform(nullptr)
{
}

GameObject::~GameObject()
{
  // destroy all components
  for (auto comp : m_components)
    delete comp;

  delete m_transform;
}

void GameObject::Initialise()
{
  // intialise any values that need to be set

  // initialize the transform
  m_transform = new TransformComponent();

  m_initialised = true;
}

void GameObject::Update(float a_dt)
{
  for (auto comp : m_components)
    comp->Update(a_dt);
}

void GameObject::AddComponent(GameobjectComponent * a_component)
{
  if (!a_component->HasBeenInitialised())
    a_component->Initialise();

  a_component->SetOwner(this);
  m_components.push_back(a_component);
}