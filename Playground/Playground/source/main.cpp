#include "World.h"

int main()
{
  // initalize the world
  printf("Initializing world..\n");
  World.InitializeWindow("Playground");
  printf("World initialized..\n");

  GameObject go;
  go.GetTransform().SetPosition(glm::vec3(100, 0, 0));
  World.AddGameobject(go);

  while (World.GetWindow().isOpen())
  {
    World.Update();
    World.Render();
  }

  printf("Exiting program..");
  return 0;
}