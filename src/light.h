#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

typedef struct
{
	vec3_t direction;
} light_directional_t;

extern light_directional_t light_directional;

#endif
