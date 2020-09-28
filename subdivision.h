#if !defined(__subdivision_h__)
#define __subdivision_h__

#include "trimesh.h"

void LoopSubdivision(trimesh::trimesh_t& mesh);
void ButterflySubdivision(trimesh::trimesh_t& mesh);

#endif // __subdivision_h__
