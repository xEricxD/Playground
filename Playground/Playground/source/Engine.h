#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Camera.h"

// TODO - add functionality for scene switching (via GUI or button presses?)
class CollisionSystem;

class EngineSingleton
{
public:
  static EngineSingleton& GetEngine()
  {
    static EngineSingleton engine;
    return engine;
  }

  void ShutDown();

  void InitEngine();

  void InitializeWindow(sf::String a_title, sf::Vector2u a_size = sf::Vector2u(800, 600));

  void Update();


  sf::RenderWindow &GetWindow() { return m_window; }
  sf::View &GetViewport() { return m_viewport; }

  std::vector<GameObject*> GetGameObjectsByType(GameObject::GameObjectType a_type);

  const float GetWorldDeltaTime() { return m_dt; }

  Camera& GetActiveCamera() { return m_mainCamera; }
  CollisionSystem* const GetCollisionSystem() { return m_collisionSystem; }

private:
  EngineSingleton() {}
  ~EngineSingleton() {}

  // Called when window is initialized
  void InitializeViewport(sf::Vector2f a_size);

  std::vector<Scene*> m_scenes;
  Scene* m_activeScene;

  // window variables
  sf::RenderWindow m_window; 
  sf::View m_viewport;

  Camera m_mainCamera;
  CollisionSystem* m_collisionSystem;

  sf::Clock m_clock;
  float m_lastFrameTime;
  float m_dt;
};

#define Engine EngineSingleton::GetEngine()

