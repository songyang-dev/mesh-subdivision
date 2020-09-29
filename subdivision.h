#if !defined(__subdivision_h__)
#define __subdivision_h__

#include "trimesh.h"
#include "Eigen/Dense"

// the omega parameter in subdivision formula
const double BUTTERFLY_PARAMETER = 1.0 / 16;

void loopSubdivision(trimesh::trimesh_t& mesh);
void butterflySubdivision(trimesh::trimesh_t& mesh);

Eigen::Vector3d midpointButterfly(trimesh::trimesh_t& mesh, 
    const trimesh::trimesh_t::halfedge_t& halfedge);

#endif // __subdivision_h__
