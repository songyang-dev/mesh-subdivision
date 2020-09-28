#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include "trimesh.h"
#include "argparse.h"

#include "subdivision.h"
#include "main.h"

int main(int argc, char *argv[])
{
    // get information on what to do
    Operation op = parse(argc, argv);

    // create a mesh from a file in op.meshName
    trimesh::trimesh_t mesh(op.meshName);
    //readMesh(mesh, op.meshName);

    // process the mesh
    int vertexCount = mesh.Vertices.rows();
    int faceCount = mesh.Faces.rows();
    processMesh(mesh, vertexCount, faceCount, op);

    // output the mesh
    auto newFaces = mesh.get_faces();
    igl::writeOBJ(op.output, mesh.Vertices, newFaces);

    // Plot the mesh
    displayMesh(mesh.Vertices, newFaces);
}

/**
 * Reads a mesh from path and stores it in the given parameters
 */
/*
void readMesh(trimesh::trimesh_t &mesh, std::string &path)
{
    igl::read_triangle_mesh(path, *mesh.Vertices, *mesh.Faces);

    auto vertices = mesh.Vertices;
    auto faces = mesh.Faces;

    // half-edges example
    std::vector<trimesh::triangle_t> triangles;

    int vertexCount = vertices->rows();
    int kNumFaces = faces->rows();
    triangles.resize(kNumFaces);
    for (int i = 0; i < kNumFaces; ++i)
    {
        triangles[i].v[0] = (*faces)(i, 0);
        triangles[i].v[1] = (*faces)(i, 1);
        triangles[i].v[2] = (*faces)(i, 2);
    }

    trimesh::unordered_edges_from_triangles(triangles.size(), &triangles[0], *mesh.Edges);

    mesh.build(vertexCount, triangles.size(), &triangles[0], mesh.Edges->size(), &(*mesh.Edges)[0]);
}
*/

/**
 * Processes the mesh according to the operations specified in the command line 
 */
void processMesh(trimesh::trimesh_t &mesh, const int vertexCount, const int faceCount,
                 Operation op)
{
    switch (op.scheme)
    {
    case Loop:
        LoopSubdivision(mesh);
        break;

    case Butterfly:
        ButterflySubdivision(mesh);
        break;

    case Sqrt3:
        std::cerr << "Sqrt3 not implemented" << std::endl;
        exit(EXIT_FAILURE);
        break;

    default:
        std::cerr << "Unknown scheme: " << op.scheme << std::endl;
        exit(EXIT_FAILURE);
    }
    // neighbor loop demonstration
    // std::vector<trimesh::index_t> neighs;
    // for (int vi = 0; vi < vertexCount; ++vi)
    // {
    //     mesh.vertex_vertex_neighbors(vi, neighs);

    //     std::cout << "neighbors of vertex " << vi << ": ";
    //     for (int i = 0; i < neighs.size(); ++i)
    //     {
    //         std::cout << ' ' << neighs.at(i);
    //     }
    //     std::cout << '\n';
    // }
}

/**
 * Shows the mesh on a libigl window, which offers interactive viewing
*/
void displayMesh(Eigen::MatrixXd &vertices, Eigen::MatrixXi &faces)
{
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(vertices, faces);
    viewer.data().set_face_based(true);
    const Eigen::RowVector3d red = Eigen::RowVector3d(1, 0, 0);
    // add vertices highlights
    viewer.data().point_size = 20;
    viewer.data().add_points(vertices, red);
    // add vertices index
    for (int i = 0; i < vertices.rows(); ++i)
    {
        viewer.data().add_label(vertices.row(i) + Eigen::RowVector3d(0.005, 0.005, 0), std::to_string(i));
    }
    viewer.data().show_custom_labels = true;
    // launch viewer
    viewer.launch();
}