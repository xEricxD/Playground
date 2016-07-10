#include "GameObject.h"
#include "Camera.h"
#include "Engine.h"

// TODO - move construct of transform to constructor, and update all gameobjects initialises once they have all been created

GameObject::GameObject() : m_initialised(false), m_transform(nullptr), m_gameObjectType(GameObjectType::NONE)
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

void GameObject::ShutDown()
{
  for (auto comp : m_components)
    comp->ShutDown();
}

void GameObject::AddComponent(GameobjectComponent * a_component)
{
  a_component->SetOwner(this);

  if (!a_component->HasBeenInitialised())
    a_component->Initialise();

  a_component->GetTransform()->SetParent(GetTransform());

  m_components.push_back(a_component);
}

std::vector<GameobjectComponent*> GameObject::GetComponentsByType(GameobjectComponent::ComponentType a_type)
{
  std::vector<GameobjectComponent*> returnVec;

  // search through all the components and add the ones with the correct type to the return vector
  for (auto comp : m_components)
  {
    if (comp->GetType() == a_type)
      returnVec.push_back(comp);
  }

  return returnVec;
}
