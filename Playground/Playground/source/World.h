#pragma once
#include <SFML/Graphics.hpp>

class WorldSingleton
{
public:
  static WorldSingleton& GetWorld()
  {
    static WorldSingleton world;
    return world;
  }

  void Update(float a_dt);
  void InitializeWindow(sf::String a_title, sf::Vector2u a_size = sf::Vector2u(800, 600));

  sf::RenderWindow& GetWindow() { return m_window; }
  sf::View& GetViewport() { return m_viewport; }

  float GetWorldDeltaTime() { return m_dt; }

private:
  WorldSingleton() {}
  ~WorldSingleton() {}

  // Called when window is initialized
  void InitializeViewport(sf::Vector2f a_size);


  // window variables
  sf::RenderWindow m_window; 
  sf::View m_viewport;

  sf::Clock m_clock;
  float m_lastFrameTime;
  float m_dt;
};

#define World WorldSingleton::GetWorld()
