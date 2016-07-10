#include "TestScene.h"
#include "Engine.h"
#include "AgentObject.h"
#include "GridObject.h"
#include "AStarPathfinderObject.h"
#include "ConvexCollisionComponent.h"
#include "CircleCollisionComponent.h"

#define CHARACTER_SPEED 20

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

  GridObject* grid = new GridObject(glm::vec2(64, 64), glm::vec2(-32, -32), glm::vec2(64, 64));
  AddGameobject(grid);
  grid->SetDrawDebug(true, true);

  AStarPathfinderObject* pathfinder = new AStarPathfinderObject();
  AddGameobject(pathfinder);

  m_agent = new AgentObject();
  AddGameobject(m_agent);
  m_agent->GetNavigationComponent()->SetDrawDebug(true);
  ConvexCollisionComponent* collider = new ConvexCollisionComponent();
  m_agent->AddComponent(collider);
  collider->SetDrawDebug(true);

  AgentObject* agent = new AgentObject();
  AddGameobject(agent);
  agent->GetTransform()->SetLocalPosition(glm::vec3(200, 0, 0)); 
  collider = new ConvexCollisionComponent();
  agent->AddComponent(collider);
  collider->SetDrawDebug(true);
}

void TestScene::Update(float a_dt)
{
  Scene::Update(a_dt);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    m_agent->GetTransform()->Translate(glm::vec3(-CHARACTER_SPEED * a_dt, 0, 0));
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    m_agent->GetTransform()->Translate(glm::vec3( CHARACTER_SPEED * a_dt, 0, 0));
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    m_agent->GetTransform()->Translate(glm::vec3(0, -CHARACTER_SPEED * a_dt, 0));
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    m_agent->GetTransform()->Translate(glm::vec3(0, CHARACTER_SPEED * a_dt, 0));
}

void TestScene::ShutDown()
{
  Scene::ShutDown();

}
