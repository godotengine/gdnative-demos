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

public:
	MeshTree();
	~MeshTree();

	void _init();
	void _ready();

	void build_tree();

	static void _register_methods();
};

} // namespace godot

#endif /* MESH_TREE_H */
