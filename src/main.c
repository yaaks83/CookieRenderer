#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "culling.h"
#include "sort.h"
#include "matrix.h"
#include "light.h"
#include "shaders.h"
#include "triangle.h"
#include "texture.h"

// Array of triangles to be rendered frame by frame
triangle_t* triangles_to_render = NULL;

// Global variables for execution status and game loop
bool is_running = false;
int previous_frame_time = 0;

vec3_t camera_position = {.x = 0,.y = 0,.z = 0};
mat4_t proj_matrix;

// Setup function to initialize variables and game objects
void setup(void)
{
	// Initialize render mode and triangle culling cull_method
	render_method = RENDER_WIRE;
	cull_method = CULL_BACKFACE;
	sort_method = SORT_QUICK;
	// Allocate the required memory in bytes to hold the color buffer
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

	//Create an SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture
	(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	// Initialize the perspective projection matrix
	float fov = M_PI / 3.0; 	// 60 deg in radians
	float aspect = (float) window_height / window_width;
	float znear = 0.1;
	float zfar = 1000.0;
	proj_matrix = mat4_perspective(fov, aspect, znear, zfar);

	// Maually load the hardcoded texture data from the static array
	mesh_texure = (uint32_t*)REDBRICK_TEXTURE;
	texture_width = 64;
	texture_height = 64;

	load_cube_mesh_data();
	//load_obj_mesh_dat("./assets/cube.obj");
	//load_obj_mesh_dat("./assets/Gilneas_Ext.obj");
	//load_obj_mesh_dat("./assets/obj_mcreature_free.obj");
}

void process_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
			is_running = false;
			break;

		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
			if(event.key.keysym.sym == SDLK_1)
				render_method = RENDER_WIRE_VERTEX;
			if(event.key.keysym.sym == SDLK_2)
				render_method = RENDER_WIRE;
			if(event.key.keysym.sym == SDLK_3)
				render_method = RENDER_FILL_TRIANGLE;
			if(event.key.keysym.sym == SDLK_4)
				render_method = RENDER_FILL_TRAINGLE_WIRE;
			if(event.key.keysym.sym == SDLK_5)
				render_method = RENDER_TEXTURED;
			if(event.key.keysym.sym == SDLK_6)
				render_method = RENDER_TEXTURED_WIRE;
			if(event.key.keysym.sym == SDLK_c)
				if(cull_method == CULL_BACKFACE)
				{
					cull_method = CULL_NONE;
				}
				else
				{
					cull_method = CULL_BACKFACE;
				}

			if(event.key.keysym.sym == SDLK_s)
				if(sort_method == SORT_QUICK)
				{
					sort_method = SORT_NONE;
				}
				else
				{
					sort_method = SORT_QUICK;
				}
			break;
	}
}



// Update function frame by frame with a fixed time step
void update(void)
{
		// Wait some time until the reach the target frame time in milliseconds
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

	// Only delay execution if we are running too fast
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
	{
	    SDL_Delay(time_to_wait);
	}

	previous_frame_time = SDL_GetTicks();

	// Initialize the arry of triangles to renderer
	triangles_to_render = NULL;

	//mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	//mesh.rotation.z += 0.01;

	//mesh.scale.x += 0.001;
	//mesh.scale.y += 0.001;
	//mesh.scale.z += 0.001;

	//mesh.translation.x += 0.01;
	mesh.translation.z = 100.0;

	mat4_t scale_matrix = mat4_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);

	mat4_t rotation_matrix_x = mat4_rotation_x(mesh.rotation.x);
	mat4_t rotation_matrix_y = mat4_rotation_y(mesh.rotation.y);
	mat4_t rotation_matrix_z = mat4_rotation_z(mesh.rotation.z);

	mat4_t translation_matrix = mat4_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

	// Loop all triangle faces of our mesh
	int num_faces = array_length(mesh.faces);

 	for(int i = 0; i < num_faces; i++)
	{
		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		vec4_t transformed_vertices[3];
		//Loop all three verticies of the current face and apply transformations
		for(int j = 0; j < 3; j++)
		{
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);
			// Create a world matrix combining scale, rotation, and translation natricies
			mat4_t world_matrix = mat4_identity();

			world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
			world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);
			// Multiply the world matrix by the original vector
			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

			// Save transformed vertex in the array of transformed vertices
			transformed_vertices[j] = transformed_vertex;
		}
			// Check backface culling
			vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]); /*	A      */
			vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]); /*   /   \    */
			vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]); /* C ---- B   */

			// Get the vector subtraction of B-A and C-A
			vec3_t vector_ab = vec3_sub(vector_b, vector_a);
			vec3_t vector_ac = vec3_sub(vector_c, vector_a);
			vec3_normalized(&vector_ab);
			vec3_normalized(&vector_ac);

			// Get the face normal between ab and ac by using the cross product
			vec3_t normal = vec3_cross(vector_ab, vector_ac);
			vec3_normalized(&normal);

			/* Find the camera ray vector by subtractiong the camera camera_position
			 and a point in the triangle*/
			vec3_t camera_ray = vec3_sub(camera_position, vector_a);
			// Get the dot product between the camera ray and the face normal
			float dot_normal_camera = vec3_dot(normal, camera_ray);
			// Ignore projection for the triangles are looking away from the camera
			if(cull_method == CULL_BACKFACE)
			{
				if(dot_normal_camera < 0)
				{
					continue;
				}
			}


		vec4_t projected_points[3];
		// Loop all the 3 vertices to perform projection
		for(int j = 0; j < 3; j++)
		{
		     //Project the current vertex
	  		projected_points[j] = mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);

			// Scale into the view
			projected_points[j].x *= (window_width/2.0);
			projected_points[j].y *= (window_height/2.0);

			//Invert the Y values to account for the flipped screen y coordinates
			projected_points[j].y *= -1;

			// Translate projected points to the middle of the screen
			projected_points[j].x += (window_width/2.0);
			projected_points[j].y += (window_height/2.0);
		}
		// Calculate the avg depth for each vertexx after transformation
		float avg_depth =
		(
			transformed_vertices[0].z +
			transformed_vertices[1].z +
			transformed_vertices[2].z
		) / 3.0 ;

		// Apply flat shading
		uint32_t triangle_color = flat_shading(normal, light_directional.direction, mesh_face.color);

		triangle_t projected_triangle =
		{
			.points =
			{
				{projected_points[0].x, projected_points[0].y},
				{projected_points[1].x, projected_points[1].y},
				{projected_points[2].x, projected_points[2].y}
			},
			.texcoords =
			{
				{mesh_face.a.uv.u, mesh_face.a.uv.v},
				{mesh_face.b.uv.u, mesh_face.b.uv.v},
				{mesh_face.c.uv.u, mesh_face.c.uv.v}
			},
			.color = triangle_color,
			.avg_depth = avg_depth
		};
		//Save the projected triangle in the array of triangles to render
		//triangles_to_render[i] = projected_triangle;
		array_push(triangles_to_render, projected_triangle);
	}

	if(sort_method == SORT_QUICK)
	{
		//Sort the triangles to render by their avg avg_depth
		int num_triangles = array_length(triangles_to_render);

		//quicksort_triangles(triangles_to_render, 0, num_triangles);

		for (int i = 0; i < num_triangles; i++)
		{
    			for (int j = i; j < num_triangles; j++)
			{
	   			if (triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth)
				{
					  // Swap the triangles positions in the array
					  triangle_t temp = triangles_to_render[i];
					  triangles_to_render[i] = triangles_to_render[j];
					  triangles_to_render[j] = temp;
	   			}
    			}
		}
	}

}

//Funtion that recieves a 3D vector abd returns a projected 2Dpoint
void render(void)
{
	SDL_RenderClear(renderer);
	draw_grid(40, 0xFF333333);
	//Loop all projected triangles and render them
	int num_triangles = array_length(triangles_to_render);

	for(int i = 0; i < num_triangles; i++)
	{
		triangle_t triangle = triangles_to_render[i];

		// Draw triangle filled
		if(
			render_method == RENDER_FILL_TRIANGLE ||
			render_method == RENDER_FILL_TRAINGLE_WIRE
		)
		{
			draw_filled_triangle
			(
				triangle.points[0].x,
				triangle.points[0].y,
				triangle.points[1].x,
				triangle.points[1].y,
				triangle.points[2].x,
				triangle.points[2].y,
				//0xFF00FF00
				triangle.color
			);
		}
		// Draw triangle wireframe
		if
		(
			render_method == RENDER_WIRE ||
			render_method == RENDER_WIRE_VERTEX ||
			render_method == RENDER_FILL_TRAINGLE_WIRE ||
			render_method == RENDER_TEXTURED_WIRE
		)
		{
			draw_triangle
			(
				triangle.points[0].x,
				triangle.points[0].y,
				triangle.points[1].x,
				triangle.points[1].y,
				triangle.points[2].x,
				triangle.points[2].y,
				0xFFFFFFFF
			);
		}
		// Draw triangle vertices
		if (render_method == RENDER_WIRE_VERTEX)
		{
			draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFFFFFF00);
			draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFFFFFF00);
			draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFFFF00);
		}
		if (render_method == RENDER_TEXTURED || render_method == RENDER_TEXTURED_WIRE)
		{
			draw_textured_triangle
			(
				triangle.points[0].x,
				triangle.points[0].y,
				triangle.texcoords[0].u,
				triangle.texcoords[0].v,
				triangle.points[1].x,
				triangle.points[1].y,
				triangle.texcoords[1].u,
				triangle.texcoords[1].v,
				triangle.points[2].x,
				triangle.points[2].y,
				triangle.texcoords[2].u,
				triangle.texcoords[2].v,
				mesh_texure
			)
		}


	}
	//draw_filled_triangle(300,100,50,400,500,700, 0xFF00FF00);
	//Clear tge array of triangles to render every frame loop
	array_free(triangles_to_render);

	render_color_buffer();
	clear_color_buffer(0XFF000000);

	SDL_RenderPresent(renderer);
}

// Free memory that was dynamically allocated by the program
void free_resources()
{
	free(color_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
}

int main(int argc, char *argv[])
{
	is_running = initialize_window();

	setup();

	while(is_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();
	free_resources();

	return 0;
}
