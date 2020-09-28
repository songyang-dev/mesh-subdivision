#if !defined(__main_h__)
#define __main_h__

#include "trimesh.h"
#include "argparse.h"

//void readMesh(trimesh::trimesh_t &mesh, std::string &path); 
    //Eigen::MatrixXd &V, Eigen::MatrixXi &F);
void processMesh(trimesh::trimesh_t& mesh, const int vertexCount, const int faceCount, Operation op);
void displayMesh(Eigen::MatrixXd& vertices, Eigen::MatrixXi& faces);

#endif // __main_h__
