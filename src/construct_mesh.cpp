#include "construct_mesh.h"

Mesh construct_cube() {

    // allocate memory for the mesh object
    Mesh cube_mesh;
    //Allocate vertices here
    cube_mesh.vertices = {
        // positions         // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Back face
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Front face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // Left face
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,

         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // Right face
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom face
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // Top face
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f
    };

    //Allocate Indices here
    cube_mesh.indices = {  // note that we start from 0!
        //Back face
        0, 1, 3,  // first Triangle
        0, 3, 2,   // second Triangle.. etc.
        // Front face
        4, 5, 7,
        4, 7, 6,
        // Left face
        8, 9, 11,
        8, 11, 10,
        // Right face
        12, 13, 15,
        12, 15, 14,
        // Bottom face
        16, 17, 19,
        16, 19, 18,
        // Top face
        20, 21, 23,
        20, 23, 22
    };

    cube_mesh.num_of_indices = cube_mesh.indices.size();

    return cube_mesh;
}

Mesh construct_pyramid() {
    Mesh pyramid_mesh;

    // Define the vertices of the pyramid
    pyramid_mesh.vertices = {
        // Base - just like the bottom face of the cube
        -0.5f, 0.0f, -0.5f, 0.0f, 0.0f,
         0.5f, 0.0f, -0.5f, 1.0f, 0.0f,
         0.5f, 0.0f,  0.5f, 1.0f, 1.0f,
        -0.5f, 0.0f,  0.5f, 0.0f, 1.0f,

        // Apex - centered above the base
         0.0f, 1.0f,  0.0f, 0.5f, 0.5f // The texture coordinates for the apex are arbitrary
    };

    // Define the indices for the pyramid
    // The base uses the first 4 vertices (0-3), and the apex is the 5th vertex (4)
    pyramid_mesh.indices = {
        // Base
        0, 1, 2,
        0, 2, 3,

        // Sides - each side is a triangle composed of two base vertices and the apex
        0, 1, 4, // Front
        1, 2, 4, // Right
        2, 3, 4, // Back
        3, 0, 4  // Left
    };

    pyramid_mesh.num_of_indices = pyramid_mesh.indices.size();

    return pyramid_mesh;
}