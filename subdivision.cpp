#include "trimesh.h"
#include "Eigen/Dense"
#include "subdivision.h"

/**
 * Loop subdivision inserts new vertices in the middle of edges
 * and adjust both new and old vertices
*/
void LoopSubdivision(trimesh::trimesh_t& mesh)
{

}

/**
 * Butterfly subdivision inserts new vertices in the middle of edges
 * and only adjusts new vertices. Old vertices stay put
*/
void ButterflySubdivision(trimesh::trimesh_t& mesh)
{

    for (int i = 0; i < mesh.Edges.size() ; i++)
    {
        
    }
    
}