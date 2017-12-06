/********************************************************************
 * MeshTree, Godot GDNative implementation for our space colonisation
 * tree generation code
 * 
 * Based on my original implementation here
 * https://github.com/BastiaanOlij/Trees
 * 
 * By Bastiaan Olij
********************************************************************/

#ifndef MESH_TREE_H
#define MESH_TREE_H

#include <ArrayMesh.hpp>
#include <Godot.hpp>
#include <Mesh.hpp>

#include "attractionpoint.h"
#include "treenode.h"

#define M_PI 3.14159265358979323846

namespace godot {

class MeshTree : public GodotScript<ArrayMesh> {
	GODOT_CLASS(MeshTree)

private:
	/* from our visual server */
	enum ArrayType {

		ARRAY_VERTEX = 0,
		ARRAY_NORMAL = 1,
		ARRAY_TANGENT = 2,
		ARRAY_COLOR = 3,
		ARRAY_TEX_UV = 4,
		ARRAY_TEX_UV2 = 5,
		ARRAY_BONES = 6,
		ARRAY_WEIGHTS = 7,
		ARRAY_INDEX = 8,
		ARRAY_MAX = 9
	};

	unsigned int seed_value;
	bool tree_is_build;

	/* attractions points */
	AttractionPoint * attraction_points;
	unsigned long max_attraction_points;
	unsigned long attraction_point_count;
	void remove_attraction_point(unsigned long p_idx);

	/* vertices for our tree skeleton */
	Vector3 bias;
	PoolVector3Array vertices;
	unsigned long last_num_of_verts;
	unsigned long add_vertex(const Vector3 & pVertex);
	void remove_vertex(unsigned long p_idx);
	unsigned long grow_branch(unsigned long pFromVertex, Vector3 pTo);

	/* nodes that form our tree */
	TreeNode * nodes;
	unsigned long max_nodes;
	unsigned long node_count;
	void add_node(TreeNode & p_node);
	void remove_node(unsigned long p_idx);

	/* building our mesh */
	struct mesh_data {
		PoolVector3Array points;
		PoolVector3Array normals;
		PoolRealArray tangents;
		PoolVector2Array uvs;
		PoolIntArray indices;
	};
	void add_rings(mesh_data *p_data, const int p_parent_node = -1, const int p_parent_first_vertex = -1, const Vector3 &p_parent_center = Vector3(0.0, 0.0, 0.0), const Vector3 &p_parent_bitangent = Vector3(1.0, 0.0, 0.0));

	/* helper functions */
	float randf(float pMin = -1.0f, float pMax = 1.0f);

public:
	MeshTree();
	~MeshTree();

	void _init();
	void _ready();

	/**
	 * clear attraction points
	 *
	 * This method clears any attraction points we have added
	 **/
	void clear_attraction_points();

	/**
	 * generate_attraction_points(p_num_of_points, p_outer_radius, p_inner_radius, p_aspect, p_offset_y, )
	 * 
	 * This method generates the attaction points for our tree. 
	 * At this moment we've only got a single very simple generation of points based on a stretched hemisphere filled with random points.
	 * The shape of our point cloud very much determines the look of our tree. 
	 * Adding more complexity to this algorithm to steer the shape of the point cloud will become a target later on.
	 *
	 * p_num_of_points - Number of attraction points (N)
	 * p_outer_radius  - Outer size of our point cloud
	 * p_inner_radius  - Inner size of our point cloud
	 * p_aspect        - Aspect ratio between height and width
	 * p_offset_y      - Y offset
	**/
	void generate_attraction_points(int p_num_of_points, float p_outer_radius, float p_inner_radius, float p_aspect, float p_offset_y);

	/**
	 * do_iteration(p_max_distance, p_branch_size, p_cut_off_distance)
	 * 
	 * This method performs one iteration of our tree generation logic and returns true if changes have been made.
	 * Basically you should repeatidly call this method until it returns false
	 * 
	 * p_max_distance     - maximum distance between attraction point and vertice for it to be considered (must be > p_inner_radius) (dk)
	 * p_branch_size      - size with which we grow a branch (D)
	 * p_cut_off_distance - once the closest distance to an attraction point and a vertice becomes less then this we remove the attraction point (di, must be a multiple of p_branch_size)
	**/
 	bool do_iteration(float p_max_distance, float p_branch_size, float p_cut_off_distance);

	/**
	 * optimiseNodes()
	 *
	 * This method will optimise nodes by joining nodes with small angles between them 
	 * Note that this will invalidate our childcount, we won't update this rather leave
	 * it up to the implementation whether to recount it or use the original counts
	 * 
	**/
 	void optimiseNodes();

	void build_tree(float p_max_distance, float p_branch_size, float p_cut_off_distance);

	static void _register_methods();
};

} // namespace godot

#endif /* MESH_TREE_H */
