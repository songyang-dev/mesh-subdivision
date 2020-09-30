#include "utils.h"

#include "trimesh_types.h"
#include <map>
#include <stdexcept>

/**
 * Gets the index of the vertex on this edge inside the given
 * map. Throws std::invalid_argument if not found.
 */
trimesh::index_t vertexFromEdge(EdgeToVertex& map, 
    const std::pair<trimesh::index_t, trimesh::index_t>& edge)
{
    // forward directed edge not found
    if (map.find(edge) == map.end())
    {
        // still not found
        auto swapped = std::make_pair(edge.second, edge.first);
        if (map.find(swapped) == map.end())
            throw std::invalid_argument("Input edge does not exist");
        else // found the opposite edge
            return map[swapped];
    }
    else
        return map[edge];
}