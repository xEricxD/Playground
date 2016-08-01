#include "Engine.h"
#include "ScenesInclude.h"
#include "CollisionSystem.h"
#include <iostream>
#include <fstream>

#define GAME_SPEED 5

void EngineSingleton::ShutDown()
{
  printf("Shutting down world..\n");

  std::ofstream file;
  file.open("../assets/saved/debug.txt", std::ios::app);

  // store a new debug session
  time_t t = time(0);   
  struct tm now;
  localtime_s(&now, &t);

  std::string dateString = std::to_string(now.tm_mday) + '-' + std::to_string(now.tm_mon + 1) + '-' + std::to_string(now.tm_year + 1900);
  std::string timeString = std::to_string(now.tm_hour) + ':' + std::to_string(now.tm_min) + ':' + std::to_string(now.tm_sec);

  file << "************************************************* New debug session ***************************************************************************\n";
  file << "Date: " << dateString << std::endl;
  file << "Time: " << timeString << std::endl;
  file << "Debug Session Length: " << std::to_string(m_clock.getElapsedTime().asSeconds()) << " seconds" << std::endl;
#ifdef _DEBUG
  file << "Running in Debug" << std::endl;
#else
  file << "Running in Release" << std::endl;
#endif

  file.close();

  for (auto scene : m_scenes)
    delete scene;

  m_collisionSystem->ShutDown();
  delete m_collisionSystem;

  file.open("../assets/saved/debug.txt", std::ios::app);
  file << "********************************************************************************************************************************************\n\n";
  file.close();

  printf("World shut down..\n");
}

void EngineSingleton::InitEngine()
{
  printf("Initializing world..\n");

  InitializeWindow("Playground", sf::Vector2u(1200, 800));
  // initialise world systems
  m_collisionSystem = new CollisionSystem();
  m_collisionSystem->Initialise();

  // Add all scenes we want in the world
  Scene* scene1 = new PathfindingScene();
  m_activeScene = scene1;
  scene1->Initialise();
  m_scenes.push_back(scene1);

  printf("World initialized..\n");
}

void EngineSingleton::Update()
{
  // handle any events in the window
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    // close the window
    if (event.type == sf::Event::Closed)
      m_window.close();

    // update the view to the new size of the window
    if (event.type == sf::Event::Resized)
    {
      m_viewport.setSize(sf::Vector2f((float)event.size.width, (float)event.size.height));
      m_window.setView(m_viewport);
    }
  }

  // update delta time
  m_dt = m_clock.getElapsedTime().asSeconds() - m_lastFrameTime;
  // cap dt at 0.5 seconds (for debugging)
  if (m_dt > 0.5f)
    m_dt = 0.5f;

  m_lastFrameTime = m_clock.getElapsedTime().asSeconds();
  
  // start update by clearing screen
  m_window.clear(sf::Color(100, 100, 255, 255));

  // update the camera and active scene
  m_mainCamera.Update(m_dt);
  m_activeScene->Update(m_dt * GAME_SPEED);
  m_collisionSystem->UpdateCollisionSystem();

  // display everything to the screen
  m_window.display();
}

std::vector<GameObject*> EngineSingleton::GetGameObjectsByType(GameObject::GameObjectType a_type)
{
  return m_activeScene->GetGameObjectsByType(a_type);
}


void EngineSingleton::InitializeWindow(sf::String a_title, sf::Vector2u a_size)
{
  m_window.create(sf::VideoMode(10, 10), "");
  m_window.setTitle(a_title);
  m_window.setSize(a_size);
  m_window.setPosition(sf::Vector2i(300, 100));

  InitializeViewport(sf::Vector2f((float)a_size.x, (float)a_size.y));

  m_window.setView(m_viewport);

  m_window.setActive();
}

void EngineSingleton::InitializeViewport(sf::Vector2f a_size)
{
  m_viewport.setSize(a_size);
  m_viewport.setCenter(sf::Vector2f(0, 0));
}
