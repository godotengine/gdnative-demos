/********************************************************************
 * Register space colonization classes
 * 
 * Based on my original implementation here
 * https://github.com/BastiaanOlij/Trees
 * 
 * By Bastiaan Olij
********************************************************************/

#include <Godot.hpp>

#include "mesh_tree.h"

using namespace godot;

GODOT_NATIVE_INIT(godot_native_init_options *options) {
	register_class<MeshTree>();
}

GODOT_NATIVE_TERMINATE(godot_native_init_options *options) {
}