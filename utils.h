#if !defined(__utils_h__)
#define __utils_h__

#include "trimesh_types.h"
#include <map>

// maps
// From edge to one vertex, in indices
typedef std::map<std::pair<trimesh::index_t, trimesh::index_t>, trimesh::index_t> 
    EdgeToVertex;
trimesh::index_t vertexFromEdge(EdgeToVertex&, 
    const std::pair<trimesh::index_t, trimesh::index_t>&);

#endif // __utils_h__
