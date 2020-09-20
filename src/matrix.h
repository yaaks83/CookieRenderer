#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef struct
{
	float m[4][4];
} mat4_t;

// Identity Matrix
mat4_t mat4_identity(void);

// Matrix Transformations
mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_translation(float tx, float ty, float tz);
mat4_t mat4_rotation_x(float angle);
mat4_t mat4_rotation_y(float angle);
mat4_t mat4_rotation_z(float angle);

// Perspective Matrix
mat4_t mat4_perspective(float fov, float aspect, float znear, float zfar);
// Perspective Divide (NDC - Normalized Device Coordinates)
vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v);

// Matrix Multiplication
vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);
mat4_t mat4_mul_mat4(mat4_t a, mat4_t b);

#endif
