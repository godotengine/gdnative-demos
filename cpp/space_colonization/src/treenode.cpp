/********************************************************************
 * class for a node in our tree
 * 
 * Based on my original implementation here
 * https://github.com/BastiaanOlij/Trees
 * 
 * By Bastiaan Olij
********************************************************************/

#include "treenode.h"

TreeNode::TreeNode() {
	a = 0;
	b = 0;
	parent = -1;
	child_count = 0;
};

TreeNode::TreeNode(unsigned long p_a, unsigned long p_b, long p_Parent) {
	a = p_a;
	b = p_b;
	parent = p_Parent;
	child_count = 0;
};

TreeNode::TreeNode(const TreeNode &p_copy) {
	a = p_copy.a;
	b = p_copy.b;
	parent = p_copy.parent;
	child_count = p_copy.child_count;
};

TreeNode &TreeNode::operator=(const TreeNode &p_copy) {
	a = p_copy.a;
	b = p_copy.b;
	parent = p_copy.parent;
	child_count = p_copy.child_count;
	return (*this);
};
