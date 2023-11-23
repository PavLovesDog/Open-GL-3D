#ifndef MESH
#define MESH

#include <vector>

//Mesh struct to hold mesh data
typedef struct Mesh {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int num_of_indices;
}Mesh;

#endif // !MESH
