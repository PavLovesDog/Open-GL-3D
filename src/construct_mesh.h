#ifndef CONSTRUCT_MESH
#define CONSTRUCT_MESH

#include "mesh.h"

Mesh construct_cube();
Mesh construct_diamond();
Mesh construct_star();
Mesh construct_sphere(unsigned int latitudeCount, unsigned int longitudeCount);

#endif // !CONSTRUCT_MESH
