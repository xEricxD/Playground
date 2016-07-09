#include "Engine.h"
#include <iostream>

int main()
{
  // initalize the world
  Engine.InitWorld();
  
  while (Engine.GetWindow().isOpen())
  {
    Engine.Update();
  }
  
  Engine.ShutDown();

  printf("Exiting program..");
  return 0;
}