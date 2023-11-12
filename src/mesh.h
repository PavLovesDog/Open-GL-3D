#ifndef MESH
#define MESH

#include <vector>

typedef struct Mesh {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int num_of_indices;
}Mesh;

#endif // !MESH
