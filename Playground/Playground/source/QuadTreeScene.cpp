#include "QuadTreeScene.h"
#include "QuadTreeObject.h"
#include "Engine.h"


QuadTreeScene::QuadTreeScene()
{
}

QuadTreeScene::~QuadTreeScene()
{
  Scene::~Scene();

}

void QuadTreeScene::Initialise()
{
  Scene::Initialise();

  // create the quadtree and add it to the scene
  QuadTreeObject* quadTree = new QuadTreeObject();
  AddGameobject(quadTree);
}

void QuadTreeScene::Update(float a_dt)
{
  Scene::Update(a_dt);

}

void QuadTreeScene::ShutDown()
{
  Scene::ShutDown();

}
