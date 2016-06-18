#include "World.h"

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
  m_lastFrameTime = m_clock.getElapsedTime().asSeconds();
  
  //Camera movement
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) m_mainCamera.Move(glm::vec3( 1, 0, 0), m_dt);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) m_mainCamera.Move(glm::vec3(-1, 0, 0), m_dt);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) m_mainCamera.Move(glm::vec3(0, -1, 0), m_dt);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) m_mainCamera.Move(glm::vec3(0,  1, 0), m_dt);

  // now update all game objects in the world
  for (auto go : m_gameObjects)
    go.Update(m_dt);
}

void WorldSingleton::Render()
{
  // start update by clearing screen
  m_window.clear(sf::Color(100, 100, 255, 255));

  for (auto go : m_gameObjects)
  {
    go.Render(m_window);
  }

  // display everything to the screen
  m_window.display();
}

void WorldSingleton::InitializeWindow(sf::String a_title, sf::Vector2u a_size)
{
  m_window.create(sf::VideoMode(10, 10), "");
  m_window.setTitle(a_title);
  m_window.setSize(a_size);
  m_window.setPosition(sf::Vector2i(500, 200));

  InitializeViewport(sf::Vector2f((float)a_size.x, (float)a_size.y));

  m_window.setView(m_viewport);

  m_window.setActive();
}

void WorldSingleton::InitializeViewport(sf::Vector2f a_size)
{
  m_viewport.setSize(a_size);
  m_viewport.setCenter(sf::Vector2f(0, 0));
}
