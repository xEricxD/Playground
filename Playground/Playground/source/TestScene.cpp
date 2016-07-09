#include "TestScene.h"
#include "Engine.h"

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
  
}

void TestScene::Update(float a_dt)
{
  Scene::Update(a_dt);

}

void TestScene::ShutDown()
{
  Scene::ShutDown();

}
