#if !defined(__subdivision_h__)
#define __subdivision_h__

#include "trimesh.h"
#include "utils.h"
#include "Eigen/Dense"

// the omega parameter in subdivision formula
const double BUTTERFLY_PARAMETER = 1.0 / 16;

// subdivision procedures
void loopSubdivision(trimesh::trimesh_t& mesh);
void butterflySubdivision(trimesh::trimesh_t& mesh);

// butterfly
Eigen::Vector3d midpointButterfly(const trimesh::trimesh_t& mesh, 
    const trimesh::trimesh_t::halfedge_t& halfedge);
std::vector<Eigen::Vector3i> facesButterfly(const trimesh::trimesh_t& mesh,
    const trimesh::index_t& face,
    EdgeToVertex& edge2Midpoints);

// loop
Eigen::Vector3d midpointLoop(const trimesh::trimesh_t& mesh, 
    const trimesh::trimesh_t::halfedge_t& halfedge);
Eigen::Vector3d adjustVertexLoop(const trimesh::trimesh_t& mesh, 
    const trimesh::index_t& vertex);

#endif // __subdivision_h__
