#if !defined(__main_h__)
#define __main_h__

#include "trimesh.h"
#include "argparse.h"

void processMesh(trimesh::trimesh_t& mesh, Operation op);
void displayMesh(Eigen::MatrixXd& vertices, Eigen::MatrixXi& faces);

#endif // __main_h__
