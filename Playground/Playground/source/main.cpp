#include "World.h"
#include <iostream>

int main()
{
  // initalize the world
  World.InitWorld();
  
  while (World.GetWindow().isOpen())
  {
    World.Update();
  }
  
  World.ShutDown();

  printf("Exiting program..");
  return 0;
}