#ifndef SHADERS_H
#define SHADERS_H

#include<stdint.h>
#include "vector.h"
#include "light.h"

// FLAT SHADING FUNCTION
uint32_t flat_shading (vec3_t normal, vec3_t direction, uint32_t original_color);

#endif
