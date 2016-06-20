#pragma once
#include "Includes.h"
#include "TransformComponent.h"

class GameObject;

class GameobjectComponent
{
public:
  // used for searching for components
  enum ComponentType
  {
    NONE = 0,

    SPRITE,
    GRIDNAVIGATION,

    COUNT
  };

  GameobjectComponent();
  virtual ~GameobjectComponent();

  virtual void Initialise();
  virtual void Update(float a_dt);

  ComponentType GetType() { return m_componentType; }
  TransformComponent* GetTransform() { return m_transform; }

  bool HasBeenInitialised() { return m_initialised; }

  GameObject* GetOwner() { return m_owner; }
  void SetOwner(GameObject* a_owner) { m_owner = a_owner; }

protected:
  ComponentType m_componentType;
  TransformComponent* m_transform;
  GameObject* m_owner;

private:
  bool m_initialised;  
};

