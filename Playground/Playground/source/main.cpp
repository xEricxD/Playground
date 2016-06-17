#include "World.h"

int main()
{
  // initalize the world
  printf("Initializing world..\n");
  World.InitializeWindow("Playground");
  printf("World initialized..\n");

  while (World.GetWindow().isOpen())
  {
    World.Update(0);
  }

  printf("Exiting program..");
  return 0;
}