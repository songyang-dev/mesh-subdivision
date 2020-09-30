#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include "trimesh.h"
#include "argparse.h"

#include "subdivision.h"
#include "main.h"

int main(int argc, char* argv[])
{
	// get information on what to do
	Operation op = parse(argc, argv);

	// create a mesh from a file in op.meshName
	trimesh::trimesh_t mesh(op.meshName);

	//debug
	//std::cout << "Vertices : " << mesh.Vertices << "Faces : " << mesh.Faces << std::endl;

	// process the mesh
	processMesh(mesh, op);

	// output the mesh
	igl::writeOBJ(op.output, mesh.Vertices, mesh.Faces);

	// Plot the mesh
	displayMesh(mesh.Vertices, mesh.Faces);
}

/**
 * Processes the mesh according to the operations specified in the command line
 */
void processMesh(trimesh::trimesh_t& mesh, Operation op)
{
	for (int i = 0; i < op.iterations; i++)
		switch (op.scheme)
		{
		case Loop:
			loopSubdivision(mesh);
			break;

		case Butterfly:
			butterflySubdivision(mesh);
			break;

		case Sqrt3:
			std::cerr << "Sqrt3 not implemented" << std::endl;
			exit(EXIT_FAILURE);
			break;

		default:
			std::cerr << "Unknown scheme: " << op.scheme << std::endl;
			exit(EXIT_FAILURE);
		}
}

/**
 * Shows the mesh on a libigl window, which offers interactive viewing
*/
void displayMesh(Eigen::MatrixXd& vertices, Eigen::MatrixXi& faces)
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