#include "SpriteComponent.h"
#include "Engine.h"

SpriteComponent::SpriteComponent()
{
  m_componentType = ComponentType::SPRITE;
}

SpriteComponent::SpriteComponent(sf::String a_textureFileName)
{
  m_componentType = ComponentType::SPRITE;
  LoadTextureFromFile(a_textureFileName);
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::Initialise()
{
  GameobjectComponent::Initialise();
}

void SpriteComponent::Update(float a_dt)
{
  GameobjectComponent::Update(a_dt);

  // render the sprite
  // use worlds active camera for transformations
  glm::mat4 modelMatrix = m_transform->GetTransformationMatrix() * GetOwner()->GetTransform()->GetTransformationMatrix();
  glm::mat4 viewMatrix = modelMatrix * glm::inverse(Engine.GetCamera().GetTransform().GetTransformationMatrix());
  
  // get the position from the matrix, and use it to draw the sprite
  glm::vec3 pos(viewMatrix[3]);
  m_sprite.setPosition(sf::Vector2f(pos.x, pos.y));

  // update the scale
  glm::vec3 scale = m_transform->GetScale();
  m_sprite.setScale(sf::Vector2f(scale.x, scale.y));

  // update rotation
  glm::vec3 rotation = GetOwner()->GetTransform()->GetRotation() + m_transform->GetRotation();
  m_sprite.setRotation(rotation.z);
  
  Engine.GetWindow().draw(m_sprite);
}

void SpriteComponent::LoadTextureFromFile(sf::String a_path)
{
  // load the texture and set the sprite data
  if (!m_texture.loadFromFile(a_path))
  {
    printf("Error: could not load texture, sprite.cpp");
    // failed to load texture, load missing texture instead
    m_texture.loadFromFile("../assets/textures/missing.png");
  }

  m_sprite.setTexture(m_texture);
  // set origin to centre of the sprite
  m_sprite.setOrigin(m_sprite.getTexture()->getSize().x * 0.5f, m_sprite.getTexture()->getSize().y * 0.5f);
}
