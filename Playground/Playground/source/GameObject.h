#pragma once
#include "Includes.h"
#include <vector>
#include "TransformComponent.h"
#include "GameobjectComponent.h"

enum GameObjectType
{
  NONE = 0,

  COUNT
};

class GameObject
{
public:
  GameObject();
  virtual ~GameObject();

  virtual void Initialise();
  virtual void Update(float a_dt);

  bool HasBeenInitialised() { return m_initialised; }

  TransformComponent* GetTransform() { return m_transform; }

  // Add a new component to the gameobject. The component will be initiliased in this function
  void AddComponent(GameobjectComponent* a_component);

  // Get all components of a certain type attached to the gameobject
  std::vector<GameobjectComponent*> &GetComponentsByType(ComponentType a_type);

protected:
  TransformComponent* m_transform;
  GameObjectType m_gameObjectType;

private:
  std::vector<GameobjectComponent*> m_components;
  bool m_initialised;
};

