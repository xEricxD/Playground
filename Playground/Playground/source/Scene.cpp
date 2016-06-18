#include "Scene.h"
#include "World.h"

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
    delete go;
}


void Scene::AddGameobject(GameObject* a_object)
{
  if (!a_object->HasBeenInitialised())
    a_object->Initialise();

  m_gameObjects.push_back(a_object);
}