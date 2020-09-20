#ifndef VECTOR_H
#define  VECTOR_H

typedef struct
{
	float x;
	float y;
} vec2_t;

typedef struct
{
	float x;
	float y;
	float z;
} vec3_t;

typedef struct
{
	float x;
	float y;
	float z;
	float w;
} vec4_t;
//------------------------------------------------------------------------------------------
//Vector 2D funtions
//------------------------------------------------------------------------------------------
float vec2_length(vec2_t v);					// Vec2 Magnitude

vec2_t vec2_add(vec2_t a, vec2_t b);			// Vec2 Addition
vec2_t vec2_sub(vec2_t a, vec2_t b);			// Vec2 Subtraction

vec2_t vec2_mul(vec2_t v, float factor);		// Vec2 Scale up
vec2_t vec2_div(vec2_t v, float factor);		// Vec2 Scale down

float vec2_dot(vec2_t a, vec2_t b);			// Vec2 DOT product
void vec2_normalized(vec2_t* v);				// Vec2 normalized
//------------------------------------------------------------------------------------------
//Vector 3D funtions
//------------------------------------------------------------------------------------------
float vec3_length(vec3_t v); 					// Vec3 Magnitude

vec3_t vec3_add(vec3_t a, vec3_t b); 			// Vec3 Addition
vec3_t vec3_sub(vec3_t a, vec3_t b);		 	// Vec3 Subtraction

vec3_t vec3_mul(vec3_t v, float factor);		// Vec3 Scale up
vec3_t vec3_div(vec3_t v, float factor);		// Vec3 Scale down

float vec3_dot(vec3_t a, vec3_t b);			// Vec3 DOT product
vec3_t vec3_cross(vec3_t a, vec3_t b);			// Vec3 CROSS product

void vec3_normalized(vec3_t* v);				// Vec3 normalized

vec3_t vec3_rotate_x(vec3_t v, float angle); 	// Vec3 Rot X (Euler)
vec3_t vec3_rotate_y(vec3_t v, float angle); 	// Vec3 Rot Y (Euler)
vec3_t vec3_rotate_z(vec3_t v, float angle); 	// Vec3 Rot Z (Euler)

vec3_t vec3_from_vec4(vec4_t v);				// Vector conversion form Vec4 to Vec3
//------------------------------------------------------------------------------------------
//Vector 4D funtions
//------------------------------------------------------------------------------------------
vec4_t vec4_from_vec3(vec3_t v);				// Vector conversion form Vec3 to Vec4

#endif
