#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <algorithm>
#define PI 3.14159265358979323846264338327950288419716939937510582097494459072381640628620899862803482534211706798f

inline float clamp(float n, float lower, float upper) 
{
  return std::max(lower, std::min(n, upper));
}