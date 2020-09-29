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
    // array of (edge index, midpoint 3d position)
    std::vector<std::pair<edge_t, Eigen::Vector3d>> edge2Midpoints;

    // for every edge create a new vertex
    for (int i = 0; i < mesh.Edges.size(); i++)
    {

        // new vertex position
        Eigen::Vector3d midpoint;

        // get the forward half edge of the directed edge
        const trimesh_t::halfedge_t forwardHE = mesh.halfedge(
            mesh.directed_edge2he_index(
                mesh.Edges[i].start(),
                mesh.Edges[i].end()));

        midpoint = midpointButterfly(mesh, forwardHE);

        // add midpoint to arrays
        edge2Midpoints.push_back(std::make_pair(mesh.Edges[i], midpoint));
    }
}

/**
 * Computes the 3d location of a midpoint on the edge,
 * according to the Butterfly scheme.
 * 
 * Given half edge rides along the edge that we are seeking the midpoint of
*/
Eigen::Vector3d midpointButterfly(trimesh_t &mesh, const trimesh_t::halfedge_t& halfedge)
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
        // opposite, next, opposite, next, v
        mesh.halfedge(
            mesh.halfedge(mesh.halfedge(mesh.halfedge(
                halfedge.opposite_he).next_he).opposite_he).next_he
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