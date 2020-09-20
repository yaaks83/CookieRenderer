#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2)

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces [N_CUBE_FACES];

// Defines a struct for dynamic size meshes, witha array of vertices and faces
typedef struct
{
	vec3_t* vertices; 	// Dynamic array of verticies
	face_t* faces; 	// Dynamic array of faces
	vec3_t rotation; 	// Rotation with X, Y, and Z values
	vec3_t scale;		// Scale with X, Y, and Z values
	vec3_t translation;	// Translate with X, Y, and Z values
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh_data(void);
void load_obj_mesh_dat(char* filename);

#endif
