#pragma once
#include "Includes.h"
#include "TransformComponent.h"
#include <SFML/Graphics.hpp>

class GameObject
{
public:
  GameObject();
  ~GameObject();

  void Update(float a_dt);
  void Render(sf::RenderWindow &a_window);

  TransformComponent &GetTransform() { return m_transform; }

private:
  TransformComponent m_transform;

  // for now just render 1 sprite per gameobject
  sf::Texture m_texture;
  sf::Sprite m_sprite;
};

