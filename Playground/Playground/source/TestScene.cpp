#include "TestScene.h"
#include "SpriteComponent.h"

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
}

void TestScene::Update(float a_dt)
{
  Scene::Update(a_dt);

}

void TestScene::ShutDown()
{
  Scene::ShutDown();

}
