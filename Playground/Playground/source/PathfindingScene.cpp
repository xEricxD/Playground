#include "PathfindingScene.h"
#include "SpriteComponent.h"
#include "GridObject.h"
#include "AStarPathfinderObject.h"
#include "JPSPathfinderObject.h"
#include "AgentObject.h"
#include "Engine.h"

#define NUM_AGENTS 1000
//#define USE_DEBUG_AGENT

//#define START_END_BY_NODE
#define START_NODE glm::vec2(45, 42)
#define GOAL_NODE glm::vec2(42,39)

#define USE_ASTAR

PathfindingScene::PathfindingScene()
{
}

PathfindingScene::~PathfindingScene()
{
  Scene::~Scene();

}

void PathfindingScene::Initialise()
{
  Scene::Initialise();

  GridObject* grid = new GridObject(glm::vec2(64, 64), glm::vec2(-32, -32), glm::vec2(64, 64));
  AddGameobject(grid);
  grid->SetDrawDebug(true, true);
  grid->LoadGridFromFile();

#ifdef USE_ASTAR
  PathfinderObject* pathfinder = new AStarPathfinderObject();
#else
  PathfinderObject* pathfinder = new JPSPathfinderObject();
#endif // USE_ASTAR

  AddGameobject(pathfinder);

#ifdef USE_DEBUG_AGENT
  m_agent = new AgentObject();
  AddGameobject(m_agent);
  m_agent->GetNavigationComponent()->SetDrawDebug(true);

#ifdef START_END_BY_NODE

  m_agent->GetTransform()->SetLocalPosition(glm::vec3(grid->GetNodeByIndex(START_NODE)->position, 0));
  m_agent->MoveToLocation(grid->GetNodeByIndex(GOAL_NODE)->position);

#endif // START_END_BY_NODE

#else
  for (int i = 0; i < NUM_AGENTS; i++)
  {
    AgentObject* agent = new AgentObject();
    AddGameobject(agent);
    agent->GetNavigationComponent()->SetWander(true);
  }
#endif
}

void PathfindingScene::Update(float a_dt)
{
  Scene::Update(a_dt);

#ifdef USE_DEBUG_AGENT

#ifdef START_END_BY_NODE
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    glm::mat4 viewMatrix = glm::inverse(Engine.GetActiveCamera().GetTransform().GetTransformationMatrix());
    glm::vec2 drawPosition(viewMatrix[3]); // get the x and y component from the vec3

    // use (mouse pos - half screen pos) to help convert to world space
    sf::Vector2i mouse = sf::Mouse::getPosition(Engine.GetWindow());
    sf::Vector2i halfscreen = sf::Vector2i((int)(Engine.GetWindow().getSize().x * 0.5f), (int)(Engine.GetWindow().getSize().y * 0.5f));

    sf::Vector2i pos = mouse - halfscreen - sf::Vector2i((int)drawPosition.x, (int)drawPosition.y);
    m_agent->MoveToLocation(glm::vec2(pos.x, pos.y));
  }
#endif

#endif
}

void PathfindingScene::ShutDown()
{
  Scene::ShutDown();

}
