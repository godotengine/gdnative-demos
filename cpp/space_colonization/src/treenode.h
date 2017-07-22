/********************************************************************
 * class for a node in our tree
 * 
 * Based on my original implementation here
 * https://github.com/BastiaanOlij/Trees
 * 
 * By Bastiaan Olij
********************************************************************/

#ifndef TREE_NODE_H
#define TREE_NODE_H

class TreeNode {
public:
	unsigned long a; // index to our vertex buffer where our node starts
	unsigned long b; // index to our vertex buffer where our node ends
	long parent; // index to our node buffer pointing to our parent (-1 if this is a root node)
	unsigned long child_count; // number of children (including their children)

	TreeNode();
	TreeNode(unsigned long p_a, unsigned long p_b, long p_parent = -1);
	TreeNode(const TreeNode &p_copy);

	TreeNode &operator=(const TreeNode &p_copy);
};

#endif
