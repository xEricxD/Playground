#pragma once
#include "Includes.h"
#include <vector>
#include "TransformComponent.h"
#include "GameobjectComponent.h"

class GameObject
{
public:
  GameObject();
  virtual ~GameObject();

  virtual void Initialise();
  virtual void Update(float a_dt);

  bool HasBeenInitialised() { return m_initialised; }

  TransformComponent* GetTransform() { return m_transform; }

  void AddComponent(GameobjectComponent* a_component);

protected:
  TransformComponent* m_transform;

private:
  std::vector<GameobjectComponent*> m_components;
  bool m_initialised;
};

