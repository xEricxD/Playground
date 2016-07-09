#include "Scene.h"
#include "Engine.h"

Scene::Scene()
{

}

Scene::~Scene()
{
  ShutDown();
}

void Scene::Initialise()
{
}

void Scene::Update(float a_dt)
{
  // now update all game objects in the world
  for (auto go : m_gameObjects)
    go->Update(a_dt);
}

void Scene::ShutDown()
{
  // delete all gameobjects
  for (auto go : m_gameObjects)
    go->ShutDown();

  for (auto go : m_gameObjects)
    delete go;

  // empty the collision system
  Engine.GetCollisionSystem().ClearCollisionSystem();
}


void Scene::AddGameobject(GameObject* a_object)
{
  if (!a_object->HasBeenInitialised())
    a_object->Initialise();

  m_gameObjects.push_back(a_object);
}

std::vector<GameObject*> Scene::GetGameObjectsByType(GameObject::GameObjectType a_type)
{
  std::vector<GameObject*> returnVec;

  // search through all the components and add the ones with the correct type to the return vector
  for (auto go : m_gameObjects)
  {
    if (go->GetType() == a_type)
      returnVec.push_back(go);
  }

  return returnVec;
}
