#include "TestScene.h"
#include "SpriteComponent.h"
#include "GridObject.h"
#include "AStarPathfinderObject.h"
#include "AgentObject.h"
#include "World.h"

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
  
  GridObject* grid = new GridObject(glm::vec2(16, 16), glm::vec2(-8, -8), glm::vec2(64, 64));
  AddGameobject(grid);
  grid->SetDrawDebug(true, true);

  AStarPathfinderObject* pathfinder = new AStarPathfinderObject();
  AddGameobject(pathfinder);
  
  m_agent = new AgentObject();
  AddGameobject(m_agent);
  m_agent->GetNavigationComponent()->SetDrawDebug(true);
}

void TestScene::Update(float a_dt)
{
  // TODO -> some sort of update order ( or some different way to render objects, based on z-order?)
  Scene::Update(a_dt);


  // temporary check to move agent to our mouse position
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    glm::mat4 viewMatrix = glm::inverse(World.GetCamera().GetTransform().GetTransformationMatrix());
    glm::vec2 drawPosition(viewMatrix[3]); // get the x and y component from the vec3
    

    // use (mouse pos - half screen pos) to help convert to world space
    sf::Vector2i mouse = sf::Mouse::getPosition(World.GetWindow());
    sf::Vector2i halfscreen = sf::Vector2i((int)(World.GetWindow().getSize().x * 0.5f),(int)(World.GetWindow().getSize().y * 0.5f));

    sf::Vector2i pos = mouse - halfscreen - sf::Vector2i((int)drawPosition.x, (int)drawPosition.y);
    m_agent->MoveToLocation(glm::vec2(pos.x, pos.y));
  }
}

void TestScene::ShutDown()
{
  Scene::ShutDown();

}
