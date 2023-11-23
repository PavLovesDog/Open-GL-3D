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

Mesh construct_diamond() {
    Mesh diamond_mesh;

    // Define the vertices of the pyramid
    diamond_mesh.vertices = {
        // Base - just like the bottom face of the cube
        -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, // 0
         0.5f, 0.0f, -0.5f, 1.0f, 0.0f, // 1
         0.5f, 0.0f,  0.5f, 1.0f, 1.0f, // 2
        -0.5f, 0.0f,  0.5f, 0.0f, 1.0f, // 3

        // Apex - centered above the base
         0.0f, 1.0f,  0.0f, 0.5f, 0.5f, // 4 (The texture coordinates for the apex are arbitrary)

         //Bottom apex
        0.0f, -1.0f, 0.0f, 0.5f, 0.5f
    };

    // Define the indices for the pyramid
    // The base uses the first 4 vertices (0-3), and the apex is the 5th vertex (4)
    diamond_mesh.indices = {
        //// Base
        //0, 1, 2,
        //0, 2, 3,

        // Top Sides - each side is a triangle composed of two base vertices and the apex
        0, 1, 4, // Front
        1, 2, 4, // Right
        2, 3, 4, // Back
        3, 0, 4,  // Left

        //Bottom Sides
        0, 1, 5, // Front
        1, 2, 5, // Right
        2, 3, 5, // Back
        3, 0, 5  // Left
    };

    diamond_mesh.num_of_indices = diamond_mesh.indices.size();

    return diamond_mesh;
}

Mesh construct_star() {
    Mesh star_mesh;

    // Texture coordinates mapping
    float center = 0.5f; // Center of the texture
    float top = 1.0f; // Top of the texture
    float bottom = 0.0f; // Bottom of the texture
    float left = 0.0f; // Left of the texture
    float right = 1.0f; // Right of the texture

    // Define the vertices of the Star
    star_mesh.vertices = {
        //   Positions      Tex coords
        0.0f, 1.15f, 0.0f, top, top, // 0
        0.30f, 0.5f, 0.0f, right, right, // 1
        1.0f, 0.5f, 0.0f, right, top, // 2
        0.45f, 0.0f, 0.0f, right, right, // 3
        0.75f, -0.85f, 0.0f, right, right, // 4
        0.0f, -0.35f, 0.0f, bottom, bottom, // 5
        -0.75f, -0.85f, 0.0f, bottom, left, // 6
        -0.45f, 0.0f, 0.0f, left, bottom, // 7
        -1.0f, 0.5f, 0.0f, left, left, // 8
        -0.30f, 0.5f, 0.0f, left, bottom,// 9

        // Front face joint point - centered
         0.0f, 0.15f,  0.35f, center, center, // 10 (The texture coordinates for the apex are arbitrary)

        // Back face joint point
        0.0f, 0.15f, -0.35f, center, center // 11
    };

    // Define the indices for the pyramid
    // The base uses the first 4 vertices (0-3), and the apex is the 5th vertex (4)
    star_mesh.indices = {
        // Front faces
        0, 1, 10,
        1, 2, 10,
        2, 3, 10,
        3, 4, 10,
        4, 5, 10,
        5, 6, 10,
        6, 7, 10,
        7, 8, 10,
        8, 9, 10,
        9, 0, 10,

        // Back faces
        0, 1, 11,
        1, 2, 11,
        2, 3, 11,
        3, 4, 11,
        4, 5, 11,
        5, 6, 11,
        6, 7, 11,
        7, 8, 11,
        8, 9, 11,
        9, 0, 11,

    };

    star_mesh.num_of_indices = star_mesh.indices.size();

    return star_mesh;
}

Mesh construct_sphere(unsigned int latitudeCount, unsigned int longitudeCount) {
    Mesh sphereMesh;
    const float PI = 3.14159265359f;

    for (unsigned int lat = 0; lat <= latitudeCount; ++lat) {
        float phi = PI * lat / latitudeCount;
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);

        for (unsigned int lon = 0; lon <= longitudeCount; ++lon) {
            float theta = 2 * PI * lon / longitudeCount;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            // Position
            sphereMesh.vertices.push_back(cosTheta * sinPhi); // x
            sphereMesh.vertices.push_back(cosPhi);           // y
            sphereMesh.vertices.push_back(sinTheta * sinPhi); // z

            // Texture Coordinates
            sphereMesh.vertices.push_back(1 - (float)lon / longitudeCount); // u
            sphereMesh.vertices.push_back(1 - (float)lat / latitudeCount);  // v
        }
    }

    for (unsigned int lat = 0; lat < latitudeCount; ++lat) {
        for (unsigned int lon = 0; lon < longitudeCount; ++lon) {
            unsigned int first = (lat * (longitudeCount + 1)) + lon;
            unsigned int second = first + longitudeCount + 1;

            // First triangle
            sphereMesh.indices.push_back(first);
            sphereMesh.indices.push_back(second);
            sphereMesh.indices.push_back(first + 1);

            // Second triangle
            sphereMesh.indices.push_back(second);
            sphereMesh.indices.push_back(second + 1);
            sphereMesh.indices.push_back(first + 1);
        }
    }

    // assign indices data to mesh data
    sphereMesh.num_of_indices = static_cast<unsigned int>(sphereMesh.indices.size());

    return sphereMesh;
}