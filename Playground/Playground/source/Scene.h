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
  std::vector<GameObject*> GetGameObjectsByType(GameObject::GameObjectType a_type);

private:
  std::vector<GameObject*> m_gameObjects;
};

