#include "TestScene.h"
#include "SpriteComponent.h"
#include "GridObject.h"

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
  Scene::~Scene();

}

void TestScene::Initialise()
{
  Scene::Initialise();

  GameObject* go = new GameObject();
  // add the object to the scene first to make sure it is initialised
  AddGameobject(go);
  SpriteComponent* spritecomp = new SpriteComponent("../assets/textures/wizard.png");
  go->AddComponent(spritecomp);

  GridObject* grid = new GridObject(glm::vec2(4, 4), glm::vec2(-2, -2), glm::vec2(64, 64));
  AddGameobject(grid);
  grid->SetDrawDebug(true, true);
}

void TestScene::Update(float a_dt)
{
  // TODO -> some sort of update order ( or some different way to render objects, based on z-order?)
  Scene::Update(a_dt);

}

void TestScene::ShutDown()
{
  Scene::ShutDown();

}
