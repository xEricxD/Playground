#include "World.h"
#include "TestScene.h"

#define GAME_SPEED 5

void WorldSingleton::ShutDown()
{
  printf("Shutting down world..\n");

  for (auto scene : m_scenes)
    delete scene;

  printf("World shut down..\n");
}

void WorldSingleton::InitWorld()
{
  printf("Initializing world..\n");

  InitializeWindow("Playground", sf::Vector2u(1200, 800));

  // Add all scenes we want in the world
  TestScene* scene1 = new TestScene();
  m_activeScene = scene1;
  scene1->Initialise();
  m_scenes.push_back(scene1);

  printf("World initialized..\n");
}

void WorldSingleton::Update()
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
  if (m_dt > 0.5f)
    m_dt = 0.5f;

  m_lastFrameTime = m_clock.getElapsedTime().asSeconds();
  
  // start update by clearing screen
  m_window.clear(sf::Color(100, 100, 255, 255));

  // update the camera and active scene
  m_mainCamera.Update(m_dt);
  m_activeScene->Update(m_dt * GAME_SPEED);

  // display everything to the screen
  m_window.display();
}

std::vector<GameObject*> WorldSingleton::GetGameObjectsByType(GameObject::GameObjectType a_type)
{
  return m_activeScene->GetGameObjectsByType(a_type);
}


void WorldSingleton::InitializeWindow(sf::String a_title, sf::Vector2u a_size)
{
  m_window.create(sf::VideoMode(10, 10), "");
  m_window.setTitle(a_title);
  m_window.setSize(a_size);
  m_window.setPosition(sf::Vector2i(300, 100));

  InitializeViewport(sf::Vector2f((float)a_size.x, (float)a_size.y));

  m_window.setView(m_viewport);

  m_window.setActive();
}

void WorldSingleton::InitializeViewport(sf::Vector2f a_size)
{
  m_viewport.setSize(a_size);
  m_viewport.setCenter(sf::Vector2f(0, 0));
}
