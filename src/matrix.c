#include "matrix.h"
#include "vector.h"
#include<math.h>

mat4_t mat4_identity(void)
{
	mat4_t m =
	{
		{
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}
		}
	};
	return m;
}

mat4_t mat4_scale(float sx, float sy, float sz)
{
	// Scalar * Identity Matrix
	mat4_t m =
	{
		{
			{sx, 0, 0, 0},
			{0, sy, 0, 0},
			{0, 0, sz, 0},
			{0, 0,  0, 1}
		}
	};
	return m;
}

mat4_t mat4_translation(float tx, float ty, float tz)
{
	// Identity Matrix with appended [0][3], [1][3], [2][3] by tx, ty and tz
	mat4_t m =
	{
		{
			{1, 0, 0, tx},
			{0, 1, 0, ty},
			{0, 0, 1, tz},
			{0, 0, 0,  1}
		}
	};
	return m;
}

mat4_t mat4_rotation_x(float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	mat4_t m =
	{
		{
			{1, 0,  0, 0},
			{0, c, -s, 0},
			{0, s,  c, 0},
			{0, 0,  0, 1}
		}
	};
	return m;
}

mat4_t mat4_rotation_y(float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	mat4_t m =
	{
		{
			{ c, 0, s, 0},
			{ 0, 1, 0, 0},
			{-s, 0, c, 0},
			{ 0, 0, 0, 1}
		}
	};
	return m;
}

mat4_t mat4_rotation_z(float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	mat4_t m =
	{
		{
			{c, -s, 0, 0},
			{s,  c, 0, 0},
			{0,  0, 1, 0},
			{0,  0, 0, 1}
		}
	};
	return m;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v)
{
	vec4_t result;

	result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
	result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
	result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
	result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;

	return result;
}

mat4_t mat4_mul_mat4(mat4_t a, mat4_t b)
{
	mat4_t m;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m.m[i][j] =
			a.m[i][0] * b.m[0][j] +
			a.m[i][1] * b.m[1][j] +
			a.m[i][2] * b.m[2][j] +
			a.m[i][3] * b.m[3][j];
		}
	}
	return m;
}

mat4_t mat4_perspective(float fov, float aspect, float znear, float zfar)
{
	// aspect * 1/tan(fov/2)		0			0				0					//
	// 		0			1/tan(fov)		0				0					//
	// 		0				0		zfar/(zfar-znear)	(-zfar * znear)/ (zfar-znear)	//
	//		0				0			1				0					//

	mat4_t m = {{{0}}};
	m.m[0][0] = aspect * (1 / tan(fov / 2));
	m.m[1][1] = 1 / tan(fov / 2);
	m.m[2][2] = zfar / (zfar - znear);
	m.m[2][3] = (-zfar * znear)/ (zfar - znear);
	m.m[3][2] = 1.0;

	return m;
}

vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v)
{
	vec4_t result = mat4_mul_vec4(mat_proj, v);

	if(result.w != 0.0)
	{
		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
	}
	return result;
}
