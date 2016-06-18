#include "GameObject.h"
#include "Camera.h"
#include "World.h"


GameObject::GameObject()
{
  // construct a basic sprite
  if (!m_texture.loadFromFile("../assets/textures/sprite1.png"))
  {
    printf("Error: could not load texture, gameobject.cpp");
  }
  m_sprite.setTexture(m_texture);
  m_sprite.setPosition(sf::Vector2f(0, 0));
  // set origin to centre of the sprite
  m_sprite.setOrigin(m_sprite.getTexture()->getSize().x * 0.5f, m_sprite.getTexture()->getSize().y * 0.5f);
}

GameObject::~GameObject()
{
}

void GameObject::Update(float a_dt)
{

}

void GameObject::Render(sf::RenderWindow &a_window)
{
  // use worlds active camera for transformations
  glm::mat4 matrix = m_transform.GetTransformationMatrix() * World.GetCamera().GetTransform().GetTransformationMatrix();

  // draw sprite in screen space position
  glm::vec3 pos(matrix[3]);
  m_sprite.setPosition(sf::Vector2f(pos.x, pos.y));
  a_window.draw(m_sprite);
}
