/********************************************************************
 * MeshTree, Godot GDNative implementation for our space colonisation
 * tree generation code
 * 
 * Based on my original implementation here
 * https://github.com/BastiaanOlij/Trees
 * 
 * By Bastiaan Olij
********************************************************************/

#include "mesh_tree.h"

using namespace godot;

MeshTree::MeshTree() {
	printf("Construct MeshTree\n");
};

MeshTree::~MeshTree(){

};

void MeshTree::_init() {
	printf("MeshTree::_init\n");

  /* just call for now, this will eventually be handled differently */
  build_tree();
};

void MeshTree::_ready() {
	printf("MeshTree::_ready\n");
};

void MeshTree::build_tree() {
	/* out with the old */
	int surface_count = owner->get_surface_count();
	for (int s = 0; s < surface_count; s++) {
		owner->surface_remove(0);
	};

	/* in with the new */
	Array arr;
	arr.resize(ARRAY_MAX);

	PoolVector3Array points;
	PoolVector3Array normals;
	//  PoolRealArray tangents;
	//  PoolVector2Array uvs;
	PoolIntArray indices;

	/* testing */
	points.push_back(Vector3(-1.0, -1.0, 0.0));
	normals.push_back(Vector3(0.0, 0.0, 1.0));

	points.push_back(Vector3(1.0, -1.0, 0.0));
	normals.push_back(Vector3(0.0, 0.0, 1.0));

	points.push_back(Vector3(1.0, 1.0, 0.0));
	normals.push_back(Vector3(0.0, 0.0, 1.0));

	points.push_back(Vector3(-1.0, 1.0, 0.0));
	normals.push_back(Vector3(0.0, 0.0, 1.0));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	/* apply mesh */
	arr[ARRAY_VERTEX] = points;
	arr[ARRAY_NORMAL] = normals;
	//  arr[ARRAY_TANGENT] = tangents;
	//  arr[ARRAY_TEX_UV] = uvs;
	arr[ARRAY_INDEX] = indices;

	owner->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arr);
};

void MeshTree::_register_methods() {
	register_method((char *)"_init", &MeshTree::_init);
	register_method((char *)"_ready", &MeshTree::_ready);
};
