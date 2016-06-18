#pragma once
#include "GameobjectComponent.h"
#include <SFML/Graphics.hpp>


class SpriteComponent : public GameobjectComponent
{
public:
  SpriteComponent();
  SpriteComponent(sf::String a_textureFileName);

  virtual ~SpriteComponent();

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;

  void LoadTextureFromFile(sf::String a_path);

private:
  sf::Sprite m_sprite;
  sf::Texture m_texture;
};

