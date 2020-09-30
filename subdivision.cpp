#include "trimesh.h"

#include "subdivision.h"

using namespace trimesh;

/**
 * Loop subdivision inserts new vertices in the middle of edges
 * and adjust both new and old vertices
*/
void loopSubdivision(trimesh::trimesh_t &mesh)
{
}

/**
 * Butterfly subdivision inserts new vertices in the middle of edges
 * and only adjusts new vertices. Old vertices stay put
*/
void butterflySubdivision(trimesh::trimesh_t &mesh)
{
    // map of (edge, midpoint 3d position)
    std::vector<Eigen::Vector3d> midpoints;

    // map from edge to vertex
    EdgeToVertex edge2Midpoints;

    // for every edge create a new vertex
    for (size_t i = 0; i < mesh.Edges.size(); i++)
    {

        // new vertex position
        Eigen::Vector3d midpoint;

        // Current edge
        auto edge = mesh.Edges[i];

        // get the forward half edge of the directed edge
        const trimesh_t::halfedge_t forwardHE = mesh.halfedge(
            mesh.directed_edge2he_index(
                edge.start(),
                edge.end()));

        midpoint = midpointButterfly(mesh, forwardHE);

        // debug
        //std::cout << midpoint.x() << " " << midpoint.y() << " " << midpoint.z() << std::endl;

        // add midpoint to array
        midpoints.push_back(midpoint);
        edge2Midpoints[std::make_pair(edge.start(), edge.end())] = mesh.Vertices.rows() + i;
    }

    // add new vertices and new faces to the mesh
    // 1. Add new vertices at the end of the matrix
    // 2. For every old face, make 4 new faces in a new matrix
    // 3. Reassign the mesh's face matrix to the new one
    
    // add new vertices
    auto previousSize = mesh.Vertices.rows();
    mesh.Vertices.conservativeResize(mesh.Vertices.rows() + midpoints.size(), Eigen::NoChange);
    auto i = previousSize;
    for (const auto& point : midpoints)
    {
        mesh.Vertices.row(i) = point;
        i++;
    }
    assert(mesh.Vertices.rows() == previousSize + midpoints.size());

    // recreate faces
    Eigen::MatrixXi dividedFaces(4 * mesh.Faces.rows(), mesh.Faces.cols());
    // loop on all current faces
    for (Eigen::Index i = 0; i < mesh.Faces.rows(); i++)
    {
        auto smallerFaces = facesButterfly(mesh, i, edge2Midpoints);
        for (auto k : {0, 1, 2, 3})
            dividedFaces.row(4*i + k) = smallerFaces[k];
    }
    assert(dividedFaces.rows() == 4 * mesh.Faces.rows());

    // reassign the faces
    mesh.Faces = dividedFaces;

    // rebuild the half edge data structure
    mesh.rebuild();

    // debug
    // for (int i = 0; i < mesh.Vertices.rows(); i++)
    // {
    //     std::cout << "Neigbors of " << i << " : ";
    //     for (auto j : mesh.vertex_vertex_neighbors(i)) std::cout << j << " ";
    //     std::cout << std::endl;
    // }
    // std::cout << "Rows : " << mesh.Vertices.rows() << std::endl;
    // std::cout << "Faces : " << mesh.Faces.rows() << std::endl;
    // std::cout << mesh.Faces << std::endl;
}

/**
 * Makes four smaller faces from one face, using midpoints
*/
std::vector<Eigen::Vector3i> facesButterfly(const trimesh::trimesh_t& mesh,
    const index_t& face,
    EdgeToVertex& edge2Midpoints)
{
    // 4 faces, each face has 3 components
    std::vector<Eigen::Vector3i> smallerFaces;

    // half edge of the larger face
    auto halfedge = mesh.get_he_index_from_face(face);

    // Procedure for making smaller faces
    // 1. Create the small face whose vertices contain an existing vertex
    // 2. Create the small face whose vertices are all midpoints

    // 1
    // Loop across the triangle face's existing vertices
    auto iterating = halfedge;
    do {
        // existing vertex (index)
        index_t existingVertex = mesh.halfedge(iterating).to_vertex;

        // get the indices of the two midpoints around this vertex
        index_t mid1 = vertexFromEdge(edge2Midpoints, mesh.he_index2directed_edge(iterating));
        index_t mid2 = vertexFromEdge(edge2Midpoints, mesh.he_index2directed_edge(
            mesh.halfedge(iterating).next_he
        ));

        // Make a new face
        smallerFaces.push_back(Eigen::Vector3i(mid1, existingVertex, mid2));

        // leap to the next vertex
        iterating = mesh.halfedge(iterating).next_he;
    } while (iterating != halfedge);
    assert(smallerFaces.size() == 3);

    // 2
    // Create the last face, with only the midpoints
    std::vector<index_t> midpoints;
    
    // gather vertices for the face
    iterating = halfedge;
    do {
        index_t point = vertexFromEdge(edge2Midpoints, 
            mesh.he_index2directed_edge(iterating));
        midpoints.push_back(point);
        // next vertex
        iterating = mesh.halfedge(iterating).next_he;
    } while (iterating != halfedge);
    assert(midpoints.size() == 3);

    // make the last face
    Eigen::Vector3i innerFace(midpoints[0], midpoints[1], midpoints[2]);
    smallerFaces.push_back(innerFace);

    assert(smallerFaces.size() == 4);

    return smallerFaces;
}

/**
 * Computes the 3d location of a midpoint on the edge,
 * according to the Butterfly scheme.
 * 
 * Given half edge rides along the edge that we are seeking the midpoint of
*/
Eigen::Vector3d midpointButterfly(const trimesh_t &mesh, const trimesh_t::halfedge_t& halfedge)
{
    Eigen::Vector3d mid;

    // 1/2 * (d1 + d2) = 1/2 (start + end of the half edge)
    auto end = mesh.Vertices.row(halfedge.to_vertex);
    auto start = mesh.Vertices.row(
        mesh.halfedge(halfedge.opposite_he).to_vertex
    );
    mid = 0.5 * (end + start);

    // omega * (d3 + d4) = omega * (average of the common neighbors)
    auto neighbor1 = mesh.Vertices.row(
        // next, v
        mesh.halfedge(halfedge.next_he).to_vertex
    );
    auto neighbor2 = mesh.Vertices.row(
        // opposite, next, v
        mesh.halfedge(
            mesh.halfedge(
                halfedge.opposite_he).next_he
        ).to_vertex
    );
    mid += BUTTERFLY_PARAMETER * (neighbor1 + neighbor2);

    // -0.5 * omega * (d5 + d6 + d7 + d8)
    // d5, d6, d7, d8 are some of the non-shared neighbors
    auto d5 = mesh.Vertices.row(
        // next, opposite, next, v
        mesh.halfedge(
            mesh.halfedge(mesh.halfedge(
                halfedge.next_he).opposite_he).next_he
        ).to_vertex
    );
    auto d6 = mesh.Vertices.row(
        // next, next, opposite, next, v
        mesh.halfedge(
            mesh.halfedge(mesh.halfedge(mesh.halfedge(
                halfedge.next_he).next_he).opposite_he).next_he
        ).to_vertex
    );
    auto d7 = mesh.Vertices.row(
        // opposite, next, next, opposite, next, v
        mesh.halfedge(
            mesh.halfedge(mesh.halfedge(mesh.halfedge(mesh.halfedge(
                halfedge.opposite_he).next_he).next_he).opposite_he).next_he
        ).to_vertex
    );
    auto d8 = mesh.Vertices.row(
        // opposite, next, opposite, next, v
        mesh.halfedge(
            mesh.halfedge(mesh.halfedge(mesh.halfedge(
                halfedge.opposite_he).next_he).opposite_he).next_he
        ).to_vertex
    );
    mid += -0.5 * BUTTERFLY_PARAMETER * (d5 + d6 + d7 + d8);

    return mid;
}