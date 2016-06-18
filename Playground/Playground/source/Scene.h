#pragma once
#include "GameObject.h"

class Scene
{
public:
  Scene();
  virtual ~Scene();

  virtual void Initialise();
  virtual void Update(float a_dt);
  virtual void ShutDown();

  void AddGameobject(GameObject* a_object);

private:
  std::vector<GameObject*> m_gameObjects;
};

