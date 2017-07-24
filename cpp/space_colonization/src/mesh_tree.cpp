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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

using namespace godot;

MeshTree::MeshTree() {
	printf("Construct MeshTree\n");

	seed_value = time(NULL);
	attraction_points = NULL;
	attraction_point_count = 0;
	max_attraction_points = 0;

	last_num_of_verts = 1;
	vertices.push_back(Vector3(0.0, 0.0, 0.0));

	nodes = NULL;
	max_nodes = 0;
	node_count = 0;

	bias = Vector3(0.0, 0.0, 0.0);
	tree_is_build = false;
};

MeshTree::~MeshTree(){
	if (attraction_points != NULL) {
		free(attraction_points);
	};

	if (nodes != NULL) {
		free(nodes);
	};
};

void MeshTree::_init() {
	printf("MeshTree::_init\n");

	/* just call these here for now, this will eventually be handled differently */
	generate_attraction_points(1000, 4.0, 0.2, 2.0, 2.0);
	build_tree();
};

void MeshTree::_ready() {
	/* probably can remove this, not applicable to a resource node */

	printf("MeshTree::_ready\n");
};

/**
 * randf(pMin, pMax)
 *
 * This method generates a random floating point number between min
 * and max
 **/
float MeshTree::randf(float pMin, float pMax) {
	float randomNumber;
	
	randomNumber = rand() % 100000; // random number between 0 and 100000
	randomNumber /= 100000; // scale to 0.0 - 1.0
	randomNumber *= (pMax - pMin); // scale to our delta
	
	return randomNumber + pMin; // and return a number between min and max
};

void MeshTree::clear_attraction_points() {
	// just reset our counter, we may free our buffer at some point...
	attraction_point_count = 0;

	// also reset our vertices buffer, no longer applies
	last_num_of_verts = 1;
	vertices.resize(0);
	vertices.push_back(Vector3(0.0, 0.0, 0.0));

	// and reset our nodes
	if (nodes != NULL) {
		nodes = NULL;
		max_nodes = 0;
		node_count = 0;
	}

	// and mark as not build...
	tree_is_build = false;
};

void MeshTree::generate_attraction_points(int p_num_of_points, float p_outer_radius, float p_inner_radius, float p_aspect, float p_offset_y) {
	if (p_num_of_points <= 0) {
		// huh?
		return;
	} else if (tree_is_build) {
		clear_attraction_points();
	};

	// Seed our randomiser
	srand(seed_value);
	
	if (attraction_points == NULL) {
		// create our buffer
		attraction_points = (AttractionPoint *) malloc(sizeof(AttractionPoint) * p_num_of_points);
		max_attraction_points = attraction_points == NULL ? 0 : p_num_of_points;
	} else if (attraction_point_count + p_num_of_points >= max_attraction_points) {
		// increase buffer size to what we need
		attraction_points = (AttractionPoint *) realloc(attraction_points, sizeof(AttractionPoint) * (attraction_point_count + p_num_of_points));
		max_attraction_points = attraction_points == NULL ? 0 : (attraction_point_count + p_num_of_points);
	};

	if (attraction_points == NULL) {
		// should communicate we ran out of memory
		return;
	};
	
	// Add random attraction points until we reached our goal
	for (unsigned long i = 0; i < p_num_of_points; i++) {
		AttractionPoint a_point;
		
		// random normalized vector for half a hemisphere
		a_point.x = randf();
		a_point.y = randf(0.0f, 1.0f);
		a_point.z = randf();
		float l = sqrt((a_point.x * a_point.x) + (a_point.y * a_point.y) + (a_point.z * a_point.z));
		a_point.x /= l; a_point.y /= l; a_point.z /= l;
		
		// Scale it up to a random radius and stretch if needed
		float s = ((p_outer_radius - p_inner_radius) * randf(0.0f, 1.0f)) + p_inner_radius;
		a_point.x *= s; a_point.y *= s; a_point.z *= s;
		a_point.y *= p_aspect;
		a_point.y += p_offset_y;
		
		// and add it to our buffer
		attraction_points[attraction_point_count++] = a_point;
	};
};

void MeshTree::remove_attraction_point(unsigned long p_idx) {
	if (p_idx < attraction_point_count) {
		attraction_point_count--;
		for (int i = p_idx; i < attraction_point_count; i++) {
			attraction_points[i] = attraction_points[i + 1]; 
		};
	};
};

void MeshTree::add_node(TreeNode & p_node) {
	if (nodes == NULL) {
		max_nodes = 100;
		nodes = (TreeNode *) malloc(sizeof(TreeNode) * max_nodes);
	} else if (node_count + 1 >= max_nodes) {
		max_nodes += 100;
		nodes = (TreeNode *) realloc(nodes, sizeof(TreeNode) * max_nodes);
	}

	if (nodes == NULL) {
		max_nodes = 0;
		node_count = 0;
	} else {
		nodes[node_count++] = p_node;
	};
};

/**
 * addVertex(pVertex)
 *
 * Adds a vertex to our vertex array (and initializes a normal and texture coord)
 **/
unsigned long MeshTree::add_vertex(const Vector3 & pVertex) {
	vertices.push_back(pVertex);
//	mNormals.push_back(pVertex.normalized()); // just for now, this will be updates
//	mTexCoords.push_back(vec2(0.0f, 0.0f));
	
	return vertices.size()-1;
};

void MeshTree::remove_vertex(unsigned long p_idx) {
	// remove vertices
	vertices.remove(p_idx);

	// adjust our nodes
	for (unsigned long n = 0; n < node_count; n++) {
		if (nodes[n].a > p_idx) nodes[n].a--;
		if (nodes[n].b > p_idx) nodes[n].b--;
	};
};

/**
 * grow_branch(pFromVertex, pTo)
 *
 * This method will grow a branch from a given vertex
 *
 * pFromVertex - index of vertex to grow from
 * pTo         - location of new vertex to branch to
 *
 **/
unsigned long MeshTree::grow_branch(unsigned long pFromVertex, Vector3 pTo) {
	int	parent = -1;

	// Find our parent (node_count will be 0 if nodes == NULL)
	for (unsigned long p = 0; p < node_count && parent == -1; p++) {
		if (nodes[p].b == pFromVertex) {
			parent = p;
		};
	};
	
	if (parent != -1) {
		// check our vector from our parent
		Vector3 parentVector = vertices[nodes[parent].b] - vertices[nodes[parent].a];
		parentVector = parentVector.normalized();
		
		Vector3 toVector = pTo - vertices[nodes[parent].b];
		toVector = toVector.normalized();
		
		// check if we're backtracking, this can happen if we're "trapped" between two equal distanced but opposite attraction points
		float dot = parentVector.dot(toVector);
		if (dot < -0.5f) {
			// use a cross product of the two vectors 
			pTo = vertices[nodes[parent].b] + (parentVector * toVector);
		};		
	};
	
	// add our new vertice
	add_vertex(pTo);
	
	// add our node
	TreeNode newnode = TreeNode(pFromVertex, vertices.size()-1, parent);
	add_node(newnode);
	
	// now update our count
	while (parent != -1) {
		nodes[parent].child_count++;
		parent = nodes[parent].parent;
	};
	
	return vertices.size()-1;
};

bool MeshTree::do_iteration(float p_max_distance, float p_branch_size, float p_cut_off_distance) {
	unsigned long numVerts = vertices.size(); // need to know the number of vertices at the start of our process
	unsigned long i, v;
	PoolRealArray numOfAPoints;
	PoolVector3Array directions;
	PoolIntArray lastClosest;
	
	// init our temporary buffers
	for (v = 0; v < numVerts; v++) {
		numOfAPoints.push_back(0.0);
		directions.push_back(Vector3(0.0f, 0.0f, 0.0f));
		lastClosest.push_back(0);
	};
	
	// find out what our closest vertice to each attraction points is:
	i = 0;
	while (i < attraction_point_count) { // use a while loop, we'll be removing points..
		AttractionPoint a_point = attraction_points[i];
		Vector3 a_position = Vector3(a_point.x, a_point.y, a_point.z);
		
		// start with our current distance for our attraction point
		Vector3 delta = vertices[a_point.closest_vertice] - a_position;
		float currentDistance = delta.length();
		
		// as our vertices haven't moved we only need to check any new vertices
		for (v = last_num_of_verts; v < vertices.size(); v++) {
			delta = vertices[v] - a_position;
			float distance = delta.length();
			if (distance < currentDistance) {
				// this one is now our closest
				a_point.closest_vertice = v;
				currentDistance = distance;
			};
		};
		
		if (currentDistance < p_cut_off_distance) {
			// we're done with this one...
			remove_attraction_point(i);
		} else {
			// copy back our new closest vertice and advance...
			attraction_points[i].closest_vertice = a_point.closest_vertice;
			
			if (currentDistance < p_max_distance) {
				// count our vertice
				float num_closest = numOfAPoints[a_point.closest_vertice];
				num_closest += 1.0;
				numOfAPoints.set(a_point.closest_vertice, num_closest);

				Vector3 norm = a_position - vertices[a_point.closest_vertice];
				Vector3 dir = directions[a_point.closest_vertice];
				dir += norm.normalized();
				directions.set(a_point.closest_vertice, dir);
				lastClosest.set(a_point.closest_vertice, i);
			};
			
			// and advance
			i++;
		};
	};
	
	// Update our last number of vertices
	last_num_of_verts = numVerts;
	
	// Now check which vertices need to branch out...
	for (v = 0; v < numVerts; v++) {		
		if (numOfAPoints[v] > 0.0) {
			Vector3	vert = vertices[v];
			Vector3 dir = directions[v];
			dir /= numOfAPoints[v];

			float	len = dir.length();
			if (len < 0.1f) {
				// this means that our points are at opposite ends, if so we ignore the last attraction point
				
				// get the vector to our last attraction point
				AttractionPoint c_point = attraction_points[lastClosest[v]];
				Vector3 norm = Vector3(c_point.x, c_point.y, c_point.z) - vert;

				// take it out
				dir *= numOfAPoints[v];
				dir -= norm.normalized();
				dir /= numOfAPoints[v] - 1;

				// recalculate our length
				len = dir.length();
			};
			
			// and check our length again to be safe
			if (len < 0.1f) {
				// if all else fails, just add an arbitrary distance
				vert += Vector3(0.0, 1.0, 0.0);
			} else {
				dir /= len;
				dir *= p_branch_size;
				vert += dir + bias;				
			};
			directions.set(v, dir);
			
			grow_branch(v, vert);			
		};
	};
	
	// as long as we still have attraction points left we must still be growing our tree
	return attraction_point_count > 0; 
};

void MeshTree::remove_node(unsigned long p_idx) {
	if (p_idx < node_count) {
		node_count--;
		for (int i = p_idx; i < node_count; i++) {
			nodes[i] = nodes[i + 1]; 
		};
	};
};

void MeshTree::optimiseNodes() {
	unsigned long node = 0;
	PoolIntArray	children;
	bool					newNode = true;
	Vector3				parentVector;
	
	while (node < node_count) {
		int mergeWith = -1;
		
		// see if we need to update our vector because we've got a new node
		if (newNode) {
			parentVector = vertices[nodes[node].b] - vertices[nodes[node].a];
			parentVector.normalize();			
		};
		
		// we need to find out how many children we have, we can only optimise if just one is found
		children.resize(0);
		for (unsigned long n = node+1; n < node_count; n++) {
			if (nodes[n].parent == node) {
				children.push_back(n);
			};
		};
		
		// only one child? check if we need to merge
		if (children.size() == 1) {
			Vector3	childVector = vertices[nodes[children[0]].b] - vertices[nodes[children[0]].a];
			childVector.normalize();
			
			// use dot product, this gives our cosine, the closer to 1.0 the more the vectors match direction
			float dot = parentVector.dot(childVector);
			if (dot > 0.995) {
				mergeWith = children[0];
			};
		};
		
		// and merge
		if (mergeWith != -1) {
			unsigned long eraseVertice = nodes[node].b; // should be same as mNodes[mergeWith].a, this we'll erase..
			
			// copy our node b from our merge node into our current node, then remove our merged node
			nodes[node].b = nodes[mergeWith].b;
			remove_node(mergeWith);
						
			// adjust our other nodes
			for (unsigned long n = 0; n < node_count; n++) {
				if (nodes[n].parent == mergeWith) { 
					nodes[n].parent = node;
				} else if (nodes[n].parent > mergeWith) {
					nodes[n].parent--;
				};
			};

			// erase our vertice we no longer need
			remove_vertex(eraseVertice);
			
			newNode = false; // we keep checking against our original vector!
		} else {
			node++;
			newNode = true;
		};
	};
};


void MeshTree::build_tree() {
	if (tree_is_build) {
		return;
	};

	/* out with the old */
	int surface_count = owner->get_surface_count();
	for (int s = 0; s < surface_count; s++) {
		owner->surface_remove(0);
	};

	/* in with the new */
	Array arr;
	arr.resize(ARRAY_MAX);

	int count = 0;
	while (do_iteration(2.5, 0.1, 0.2) && (count < 1000)) {
		// keep going....
		count++;
	};
	optimiseNodes();


	PoolVector3Array points;
	PoolVector3Array normals;
	//  PoolRealArray tangents;
	//  PoolVector2Array uvs;
	PoolIntArray indices;

	/* testing */
	for (int i = 0; i < node_count; i++) {
		TreeNode t_node = nodes[i];
		Vector3 v_a = vertices[t_node.a];
		Vector3 v_a2 = v_a + Vector3(0.1, 0.0, 0.0);
		Vector3 v_b = vertices[t_node.b];
		Vector3 v_b2 = v_b + Vector3(0.1, 0.0, 0.0);

		points.push_back(v_a);
		normals.push_back(Vector3(0.0, 0.0, -1.0));

		points.push_back(v_a2);
		normals.push_back(Vector3(0.0, 0.0, -1.0));

		points.push_back(v_b2);
		normals.push_back(Vector3(0.0, 0.0, -1.0));

		points.push_back(v_b);
		normals.push_back(Vector3(0.0, 0.0, -1.0));

		indices.push_back((i * 4) + 0);
		indices.push_back((i * 4) + 1);
		indices.push_back((i * 4) + 2);

		indices.push_back((i * 4) + 0);
		indices.push_back((i * 4) + 2);
		indices.push_back((i * 4) + 3);
	};

	/* apply mesh */
	arr[ARRAY_VERTEX] = points;
	arr[ARRAY_NORMAL] = normals;
	//  arr[ARRAY_TANGENT] = tangents;
	//  arr[ARRAY_TEX_UV] = uvs;
	arr[ARRAY_INDEX] = indices;

	owner->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arr);

	tree_is_build = true;
};

void MeshTree::_register_methods() {
	register_method((char *)"_init", &MeshTree::_init);
	register_method((char *)"_ready", &MeshTree::_ready);

	register_method((char *)"clear_attraction_points", &MeshTree::clear_attraction_points);
	register_method((char *)"generate_attraction_points", &MeshTree::generate_attraction_points);
};
