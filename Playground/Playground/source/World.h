#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Camera.h"
#include <vector>

class WorldSingleton
{
public:
  static WorldSingleton& GetWorld()
  {
    static WorldSingleton world;
    return world;
  }

  void InitializeWindow(sf::String a_title, sf::Vector2u a_size = sf::Vector2u(800, 600));

  void Update();
  void Render();

  sf::RenderWindow& GetWindow() { return m_window; }
  sf::View& GetViewport() { return m_viewport; }

  float GetWorldDeltaTime() { return m_dt; }

  void AddGameobject(GameObject a_object) { m_gameObjects.push_back(a_object); }

  Camera &GetCamera() { return m_mainCamera; }

private:
  WorldSingleton() {}
  ~WorldSingleton() {}

  // Called when window is initialized
  void InitializeViewport(sf::Vector2f a_size);

  // window variables
  sf::RenderWindow m_window; 
  sf::View m_viewport;

  Camera m_mainCamera;

  sf::Clock m_clock;
  float m_lastFrameTime;
  float m_dt;

  std::vector<GameObject> m_gameObjects;
};

#define World WorldSingleton::GetWorld()

