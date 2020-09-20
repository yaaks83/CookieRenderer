#include<stdint.h>
#include "shaders.h"
#include "light.h"
#include "vector.h"

/* Calculate the light intensity by using DOT prodcut between the face normal
 and the lightdirection */
uint32_t flat_shading (vec3_t normal, vec3_t direction, uint32_t original_color)
{
	float light_intensity_factor = -vec3_dot(normal, direction);

	if(light_intensity_factor < 0)
	{
		light_intensity_factor = 0;
	}
	if(light_intensity_factor > 1)
	{
		light_intensity_factor = 1;
	}
	uint32_t a = (original_color & 0xFF000000);
	uint32_t r = (original_color & 0x00FF0000) * light_intensity_factor;
	uint32_t g = (original_color & 0x0000FF00) * light_intensity_factor;
	uint32_t b = (original_color & 0x000000FF) * light_intensity_factor;

	uint32_t shaded_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);

	return shaded_color;
}
