/********************************************************************
 * treelogic contains the logic to generate our tree
 * 
 * Implementation based on the "Space Colonization Algorithm" by
 * Adam Runions, Brendan Lane, and Przemyslaw Prusinkiewicz
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#ifdef NOTDEFINED

#include "treelogic.h"

/////////////////////////////////////////////////////////////////////
// class for a slice
/////////////////////////////////////////////////////////////////////

slice::slice() {
	for (int i = 0; i < 5; i++) {
		p[i] = 0;
	};
};

slice::slice(const slice& pCopy) {
	for (int i = 0; i < 5; i++) {
		p[i] = pCopy.p[i];
	};	
};

slice& slice::operator=(const slice& pCopy) {
	for (int i = 0; i < 5; i++) {
		p[i] = pCopy.p[i];
	};
	
	return (*this);
};

/////////////////////////////////////////////////////////////////////
// class for a triangle
/////////////////////////////////////////////////////////////////////

triangle::triangle() {
	for (int i = 0; i < 3; i++) {
		v[i] = 0;
	};	
};

triangle::triangle(const triangle& pCopy) {
	for (int i = 0; i < 3; i++) {
		v[i] = pCopy.v[i];
	};		
};

triangle& triangle::operator=(const triangle& pCopy) {
	for (int i = 0; i < 3; i++) {
		v[i] = pCopy.v[i];
	};		
	return (*this);
};

/////////////////////////////////////////////////////////////////////
// class for a quad
/////////////////////////////////////////////////////////////////////

quad::quad() {
	for (int i = 0; i < 4; i++) {
		v[i] = 0;
	};
};

quad::quad(const quad& pCopy) {
	for (int i = 0; i < 4; i++) {
		v[i] = pCopy.v[i];
	};	
};
	
quad& quad::operator=(const quad& pCopy) {
	for (int i = 0; i < 4; i++) {
		v[i] = pCopy.v[i];
	};	
	return (*this);
};

/////////////////////////////////////////////////////////////////////
// TreeLogic
//
// constructors/destructors
/////////////////////////////////////////////////////////////////////

/**
 * treelogic(pTrunkNode)
 * 
 * constructor for our treelogic, requires the initial vector for our first trunk node
 **/
treelogic::treelogic() {
	// set some defaults
	mLastNumOfVerts	= 1;
	
	// add our root vertex
	addVertex(vec3(0.0, 0.0, 0.0)); // our tree "root"
	
	// default our shaders
	mWireFrame = false;
	mSimpleShader = NULL;
	mTreeShader = NULL;
	mLeafShader = NULL;
	
	// init our buffers
	mVAO_APoints = 0;
	mVBO_APoints = 0;
	
	mUpdateBuffers = true;
	mVAO_Tree = 0;
	mVAO_Leaves = 0;
	mVBO_Verts = 0;
	mVBO_TreeElements = 0;
	mVBO_LeafElements = 0;
	
	// init our texture ID
	mBarkTextID = 0;
	mLeafTextID = 0;
	
	// tree generation info
	mMinRadius = 0.4f;
	mRadiusFactor = 1.0f / 400.0f;
	mLeafSize.x = 20.0f;
	mLeafSize.y = 30.0f;
};

treelogic::~treelogic() {
	// free our textures
	if (mLeafTextID != 0) {
		glDeleteTextures(1, &mLeafTextID);
		mLeafTextID = 0;
	}
	if (mBarkTextID != 0) {
		glDeleteTextures(1, &mBarkTextID);
		mBarkTextID = 0;
	};
	
	// free our attraction point objects
	if (mVAO_APoints != 0) {
		glDeleteVertexArrays(1, &mVAO_APoints);
		mVAO_APoints = 0;
	};
	if (mVBO_APoints != 0) {
		glDeleteBuffers(1, &mVBO_APoints);
		mVBO_APoints = 0;
	};

	// free our tree rendering objects
	if (mVAO_Tree != 0) {
		glDeleteVertexArrays(1, &mVAO_Tree);
		mVAO_Tree = 0;
	};
	if (mVAO_Leaves != 0) {
		glDeleteVertexArrays(1, &mVAO_Leaves);
		mVAO_Leaves = 0;
	}
	if (mVBO_Verts != 0) {
		glDeleteBuffers(1, &mVBO_Verts);
		mVBO_Verts = 0;
	};
	if (mVBO_TreeElements != 0) {
		glDeleteBuffers(1, &mVBO_TreeElements);
		mVBO_TreeElements = 0;
	};
	if (mVBO_LeafElements != 0) {
		glDeleteBuffers(1, &mVBO_LeafElements);
		mVBO_LeafElements = 0;
	};
	
	// free our shaders
	if (mLeafShader != NULL) {
		delete mLeafShader;
		mLeafShader = NULL;
	};
	if (mTreeShader != NULL) {
		delete mTreeShader;
		mTreeShader = NULL;
	};
	if (mSimpleShader != NULL) {
		delete mSimpleShader;
		mSimpleShader = NULL;
	};
};

/////////////////////////////////////////////////////////////////////
// properties
/////////////////////////////////////////////////////////////////////

bool treelogic::wireframe() {
	return mWireFrame;
};

void treelogic::setWireframe(bool pWireframe) {
	mWireFrame = pWireframe;
};

float treelogic::minRadius() {
	return mMinRadius;
};

void treelogic::setMinRadius(float pRadius) {
	mMinRadius = pRadius;
};

float treelogic::radiusFactor() {
	return mRadiusFactor;
};

void treelogic::setRadiusFactor(float pFactor) {
	mRadiusFactor = pFactor;
};

/////////////////////////////////////////////////////////////////////
// helpers
/////////////////////////////////////////////////////////////////////

/**
 * randf(pMin, pMax)
 *
 * This method generates a random floating point number between min
 * and max
 **/
float treelogic::randf(float pMin, float pMax) {
	float randomNumber;
	
	randomNumber = rand() % 100000; // random number between 0 and 100000
	randomNumber /= 100000; // scale to 0.0 - 1.0
	randomNumber *= (pMax - pMin); // scale to our delta
	
	return randomNumber + pMin; // and return a number between min and max
};

/**
 * addVertex(pVertex)
 *
 * Adds a vertex to our vertex array (and initializes a normal and texture coord)
 **/
unsigned long treelogic::addVertex(const vec3& pVertex) {
	mVertices.push_back(pVertex);
	mNormals.push_back(pVertex.normalized()); // just for now, this will be updates
	mTexCoords.push_back(vec2(0.0f, 0.0f));
	
	// make sure we update our buffers
	mUpdateBuffers = true;

	return mVertices.size()-1;
};

/**
 * remVertex(pIndex)
 *
 * Removes the vertex at index pIndex and updates related arrays
 **/
void treelogic::remVertex(unsigned long pIndex) {
	mVertices.erase(mVertices.begin() + pIndex);
	mNormals.erase(mNormals.begin() + pIndex);
	mTexCoords.erase(mTexCoords.begin() + pIndex);
	
	// adjust our other nodes
	for (unsigned long n = 0; n < mNodes.size(); n++) {
		if (mNodes[n].a > pIndex) mNodes[n].a--;
		if (mNodes[n].b > pIndex) mNodes[n].b--;
	};
	
	// adjust our elements
	for (unsigned long e = 0; e < mTreeElements.size(); e++) {
		for (int i = 0; i < 4; i++) {
			if (mTreeElements[e].v[i] > pIndex) mTreeElements[e].v[i]--;
		};
	};

	for (unsigned long e = 0; e < mLeafElements.size(); e++) {
		for (int i = 0; i < 3; i++) {
			if (mLeafElements[e].v[i] > pIndex) mLeafElements[e].v[i]--;
		};
	};
	
	// make sure we update our buffers
	mUpdateBuffers = true;
};

/////////////////////////////////////////////////////////////////////
// Matrices
/////////////////////////////////////////////////////////////////////

mat4 treelogic::projection() {
	return mProjection;
};

void treelogic::setProjection(const mat4& pProjection) {
	mProjection = pProjection;
};

mat4 treelogic::view() {
	return mView;
};

void treelogic::setView(const mat4& pView) {
	mView = pView;
};

mat4 treelogic::model() {
	return mModel;
};

void treelogic::setModel(const mat4& pModel) {
	mModel = pModel;
};

/////////////////////////////////////////////////////////////////////
// tree generation code
/////////////////////////////////////////////////////////////////////

/**
 * growBrach(pFromVertex, pTo)
 *
 * This method will grow a branch from a given vertex
 *
 * pFromVertex - index of vertex to grow from
 * pTo         - location of new vertex to branch to
 *
 **/
unsigned long treelogic::growBranch(unsigned long pFromVertex, vec3 pTo) {
	int	parent = -1;
	
	// Find our parent
	for (unsigned long p = 0; p < mNodes.size() && parent == -1; p++) {
		if (mNodes[p].b == pFromVertex) {
			parent = p;
		};
	};
	
	if (parent != -1) {
		// check our vector from our parent
		vec3 parentVector = mVertices[mNodes[parent].b] - mVertices[mNodes[parent].a];
		parentVector = parentVector.normalized();
		
		vec3 toVector = pTo - mVertices[mNodes[parent].b];
		toVector = toVector.normalized();
		
		// check if we're backtracking, this can happen if we're "trapped" between two equal distanced but opposite attraction points
		float dot = parentVector % toVector;
		if (dot < -0.5f) {
			// use a cross product of the two vectors 
			pTo = mVertices[mNodes[parent].b] + (parentVector * toVector);
		};		
	};
	
	// add our new vertice
	addVertex(pTo);
	
	// add our node
	mNodes.push_back(treenode(pFromVertex, mVertices.size()-1, parent));
	
	// now update our count
	while (parent != -1) {
		mNodes[parent].childcount++;
		parent = mNodes[parent].parent;
	};
	
	return mVertices.size()-1;
};

/**
 * generateAttractionPoints(pCount, pNumOfPoints, pOuterRadius, pInnerRadius, pAspect, pOffsetY)
 * 
 * This method generates the attaction points for our tree. 
 * At this moment we've only got a single very simple generation of points based on a stretched hemisphere filled with random points.
 * The shape of our point cloud very much determines the look of our tree. 
 * Adding more complexity to this algorithm to steer the shape of the point cloud will become a target later on.
 *
 * pNumOfPoints - Number of attraction points (N)
 * pOuterRadius - Outer size of our point cloud
 * pInnerRadius - Inner size of our point cloud
 * pAspect      - Aspect ratio between height and width
 * pOffsetY     - Y offset
 * pClear    	- Clears our attraction points first
 **/
void treelogic::generateAttractionPoints(unsigned long pNumOfPoints, float pOuterRadius, float pInnerRadius, float pAspect, float pOffsetY, bool pClear) {
	// Seed our randomiser
	srand (time(NULL));
	
	if (pClear) {
		// Clear any existing points (shouldn't be any..)
		mAttractionPoints.clear();		
	};
	
	// Add random attraction points until we reached our goal
	for (unsigned long i = 0; i < pNumOfPoints; i++) {
		vec3 point;
		
		// random normalized vector for half a hemisphere
		point.x = randf();
		point.y = randf(0.0f, 1.0f);
		point.z = randf();		
		point = point.normalized();
		
		// Scale it up to a random radius and stretch if needed
		point *= ((pOuterRadius - pInnerRadius) * randf(0.0f, 1.0f)) + pInnerRadius;
		point.y *= pAspect;
		point.y += pOffsetY;
		
		// and add it to our buffer
		mAttractionPoints.push_back(attractionPoint(point));
	};
};

/**
 * doIteration(pCutOffDistance, pBranchSize)
 * 
 * This method performs one iteration of our tree generation logic and returns true if changes have been made.
 * Basically you should repeatidly call this method until it returns false
 * 
 * pMaxDistance    - maximum distance between attraction point and vertice for it to be considered (must be > pInnerRadius) (dk)
 * pBranchSize     - size with which we grow a branch (D)
 * pCutOffDistance - once the closest distance to an attraction point and a vertice becomes less then this we remove the attraction point (di, must be a multiple of pBranchSize)
 * pBias           - vector to add to simulate the effect the direction of growth
 **/
bool treelogic::doIteration(float pMaxDistance, float pBranchSize, float pCutOffDistance, vec3 pBias) {
	unsigned long numVerts = mVertices.size(); // need to know the number of vertices at the start of our process
	unsigned long i, v;
	std::vector<float> numOfAPoints;
	std::vector<vec3> directions;
	std::vector<unsigned long> lastClosest;
	
	// init our temporary buffers
	for (v = 0; v < numVerts; v++) {
		numOfAPoints.push_back(0.0);
		directions.push_back(vec3(0.0f, 0.0f, 0.0f));
		lastClosest.push_back(0);
	};
	
	// find out what our closest vertice to each attraction points is:
	i = 0;
	while (i < mAttractionPoints.size()) { // use a while loop, we'll be removing points..
		attractionPoint point = mAttractionPoints[i];
		
		// start with our current distance for our attraction point
		vec3 delta = mVertices[point.closestVertice] - point.position;
		float currentDistance = delta.length();
		
		// as our vertices haven't moved we only need to check any new vertices
		for (v = mLastNumOfVerts; v < mVertices.size(); v++) {
			delta = mVertices[v] - point.position;
			float distance = delta.length();
			if (distance < currentDistance) {
				// this one is now our closest
				point.closestVertice = v;
				currentDistance = distance;
			};
		};
		
		if (currentDistance < pCutOffDistance) {
			// we're done with this one...
			mAttractionPoints.erase(mAttractionPoints.begin() + i);
		} else {
			// copy back our new closest vertice and advance...
			mAttractionPoints[i].closestVertice = point.closestVertice;
			
			if (currentDistance < pMaxDistance) {
				// count our vertice
				numOfAPoints[point.closestVertice] += 1.0;
				vec3 norm = mAttractionPoints[i].position - mVertices[point.closestVertice];
				directions[point.closestVertice] += norm.normalized();
				lastClosest[point.closestVertice] = i;
			};
			
			// and advance
			i++;
		};
	};
	
	// Update our last number of vertices
	mLastNumOfVerts = numVerts;
	
	// Now check which vertices need to branch out...
	for (v = 0; v < numVerts; v++) {		
		if (numOfAPoints[v] > 0.0) {
			vec3	vert = mVertices[v];
			directions[v] /= numOfAPoints[v];
			float	len = directions[v].length();
			if (len < 0.1f) {
				// this means that our points are at opposite ends, if so we ignore the last attraction point
				
				// get the vector to our last attraction point
				vec3 norm = mAttractionPoints[lastClosest[v]].position - vert;

				// take it out
				directions[v] *= numOfAPoints[v];
				directions[v] -= norm.normalized();
				directions[v] /= numOfAPoints[v] - 1;
				
				// recalculate our length
				len = directions[v].length();
			};
			
			// and check our length again to be safe
			if (len < 0.1f) {
				// if all else fails, just add an arbitrary distance
				vert += vec3(0.0, 1.0, 0.0);
			} else {
				directions[v] /= len;
				directions[v] *= pBranchSize;				
				vert += directions[v] + pBias;				
			};
			
			growBranch(v, vert);			
		};
	};
	
	// as long as we still have attraction points left we must still be growing our tree
	return mAttractionPoints.size() > 0; 
};

/**
 * optimiseNodes()
 *
 * This method will optimise nodes by joining nodes with small angles between them 
 * Note that this will invalidate our childcount, we won't update this rather leave
 * it up to the implementation whether to recount it or use the original counts
 * 
 **/
void treelogic::optimiseNodes() {
	unsigned long node = 0;
	std::vector<long> children;
	bool		newNode = true;
	vec3		parentVector;
	
	while (node < mNodes.size()) {
		int mergeWith = -1;
		
		// see if we need to update our vector because we've got a new node
		if (newNode) {
			parentVector = mVertices[mNodes[node].b] - mVertices[mNodes[node].a];
			parentVector = parentVector.normalized();			
		};
		
		// we need to find out how many children we have, we can only optimise if just one is found
		children.clear();
		for (unsigned long n = node+1; n < mNodes.size(); n++) {
			if (mNodes[n].parent == node) {
				children.push_back(n);
			};
		};
		
		// only one child? check if we need to merge
		if (children.size() == 1) {
			vec3	childVector = mVertices[mNodes[children[0]].b] - mVertices[mNodes[children[0]].a];
			childVector = childVector.normalized();
			
			// use dot product, this gives our cosine, the closer to 1.0 the more the vectors match direction
			float dot = parentVector % childVector;
			if (dot > 0.995) {
				mergeWith = children[0];
			};
		};
		
		// and merge
		if (mergeWith != -1) {
			unsigned long eraseVertice = mNodes[node].b; // should be same as mNodes[mergeWith].a, this we'll erase..
			
			// copy our node b from our merge node into our current node, then remove our merged node
			mNodes[node].b = mNodes[mergeWith].b;			
			mNodes.erase(mNodes.begin() + mergeWith);
						
			// adjust our other nodes
			for (unsigned long n = 0; n < mNodes.size(); n++) {
				if (mNodes[n].parent == mergeWith) { 
					mNodes[n].parent = node;
				} else if (mNodes[n].parent > mergeWith) {
					mNodes[n].parent--;
				};
			};

			// erase our vertice we no longer need
			remVertex(eraseVertice);
			
			newNode = false; // we keep checking against our original vector!
		} else {
			node++;
			newNode = true;
		};
	};
};

/**
 * createSlice(pCenter, pDir)
 *
 * This method creates a slice based on a center vertex and a direction vector
 *
 * pCenter		- the center of our slice
 * pPlaneNormal	- normal of our plane
 * pBitangent	- direction vector within the plane of our previous slice.
 * pSize		- size of our slice
 * pDistance	- distance "travelled" along our tree, we use this for texture coordinates
 *
 **/
slice treelogic::createSlice(vec3 pCenter, vec3 pPlaneNormal, vec3 pBitangent, float pSize, float pDistance) {
	slice newSlice;
	mat3  rotate;
	float distFact = 50.0f;
	
	// create our vertices counter clockwise
	rotate.rotate(-90.0f, pPlaneNormal);
	
	// just a safety in case our bitangent lies parallel to our normal
	if ((pBitangent % pPlaneNormal) > 0.99) {
		pBitangent = vec3(1.0, 0.0, 0.0);
		pBitangent.normalized();
	};
	
	// we use the bitangent of the previous plane to calculate our tangent
	// this lines up our starting vertex better
	vec3 tangent = pPlaneNormal * pBitangent;
	tangent = tangent.normalized();
	
	// now create our vertices
	int p = addVertex(pCenter + (tangent * pSize));
	mNormals[p] = tangent;
	mTexCoords[p] = vec2(0, pDistance / distFact);
	newSlice.p[0] = p;
	
	vec3 dirB = rotate * tangent;

	p = addVertex(pCenter + (dirB * pSize));
	mNormals[p] = dirB;
	mTexCoords[p] = vec2(0.25, pDistance / distFact);
	newSlice.p[1] = p;
	
	dirB = rotate * dirB;

	p = addVertex(pCenter + (dirB * pSize));
	mNormals[p] = dirB;
	mTexCoords[p] = vec2(0.50, pDistance / distFact);
	newSlice.p[2] = p;
	
	dirB = rotate * dirB;

	p = addVertex(pCenter + (dirB * pSize));
	mNormals[p] = dirB;
	mTexCoords[p] = vec2(0.75, pDistance / distFact);
	newSlice.p[3] = p;

	// the last vertex is in the same location as the first but with different texture coords
	p = addVertex(pCenter + (tangent * pSize));
	mNormals[p] = tangent;
	mTexCoords[p] = vec2(1.0, pDistance / distFact);
	newSlice.p[4] = p;
	
	return newSlice;
};

/**
 * capSlice(pSlice)
 *
 * Puts a cap at the end of a branch
 **/
void treelogic::capSlice(const slice& pSlice) {
	quad newQuad;

	newQuad.v[0] = pSlice.p[3];
	newQuad.v[1] = pSlice.p[2];
	newQuad.v[2] = pSlice.p[1];
	newQuad.v[3] = pSlice.p[0];

	mTreeElements.push_back(newQuad);

	// make sure we update our buffers
	mUpdateBuffers = true;
};

/**
 * joinTwoSlices(pA, pB)
 *
 * creates the quads that join these two slices
 * 
 **/
void treelogic::joinTwoSlices(const slice& pA, const slice& pB) {
	for (int i = 0; i < 4; i++) {
		quad newQuad;
		
		newQuad.v[0] = pB.p[i];
		newQuad.v[1] = pB.p[i+1];
		newQuad.v[2] = pA.p[i+1];
		newQuad.v[3] = pA.p[i];
		
		mTreeElements.push_back(newQuad);

		// make sure we update our buffers
		mUpdateBuffers = true;
	};
};

/**
 * joinMultiSlices(pSliceCount, pSlices)
 * 
 * creates quads and vertices for a split in our branches
 *
 **/
void treelogic::joinMultiSlices(long pSliceCount, slice* pSlices) {
	// for now we cheat, we just join them, but this should become a binary join of these meshes...
	for (long s = 1; s < pSliceCount; s++) {
		for (int i = 0; i < 4; i++) {
			quad newQuad;
		
			newQuad.v[0] = pSlices[s].p[i];
			newQuad.v[1] = pSlices[s].p[i+1];
			newQuad.v[2] = pSlices[0].p[i+1];
			newQuad.v[3] = pSlices[0].p[i];
		
			mTreeElements.push_back(newQuad);

			// make sure we update our buffers
			mUpdateBuffers = true;
		};		
	};
};

/**
 * addLeaves(pCenter, pDirection)
 *
 * adds our branch
 **/
void treelogic::addLeaves(vec3 pCenter, vec3 pTangent, vec3 pBiTangent) {
	GLuint v[4];
	
	vec3 normal = pTangent * pBiTangent;
	vec3 tangent = pTangent * mLeafSize.y * randf(0.8f,1.0f);
	vec3 bitangent = pBiTangent * mLeafSize.x * randf(0.8f,1.0f);
	vec3 vertex = pCenter;
		
	vertex -= bitangent * 0.5f;
	v[0] = addVertex(vertex);
	mNormals[v[0]] = normal;
	mTexCoords[v[0]] = vec2(0.0f, 1.0f);

	vertex += tangent;
	v[1] = addVertex(vertex);
	mNormals[v[1]] = normal;
	mTexCoords[v[1]] = vec2(0.0f, 0.0f);

	vertex += bitangent;
	v[2] = addVertex(vertex);
	mNormals[v[2]] = normal;
	mTexCoords[v[2]] = vec2(1.0f, 0.0f);

	vertex -= tangent;
	v[3] = addVertex(vertex);
	mNormals[v[3]] = normal;
	mTexCoords[v[3]] = vec2(1.0f, 1.0f);
	
	triangle newTriangle;
	
	newTriangle.v[0] = v[0];
	newTriangle.v[1] = v[1];
	newTriangle.v[2] = v[2];
	
	mLeafElements.push_back(newTriangle);

	newTriangle.v[0] = v[0];
	newTriangle.v[1] = v[2];
	newTriangle.v[2] = v[3];
	
	mLeafElements.push_back(newTriangle);
};

/** 
 * expandChildren(pParentNode, pOffset)
 *
 * This method expands the model based on the child nodes of a parent
 *
 * pParentNode	- the node for which we're expanding to its children (-1 means we're doing our root nodes)
 * pParentSlice	- the slice we created for our parent (note, empty if we're doing our rootnodes)
 * pOffset		- vector by which to offset what we're creating
 * pDistance	- distance "travelled" along our tree, we use this for texture coordinates
 *
 **/
void treelogic::expandChildren(unsigned long pParentNode, const slice& pParentSlice, vec3 pOffset, float pDistance) {
	// find out how many child nodes we have
	std::vector<int> childNodes;
	
	for (int n = 0; n < mNodes.size(); n++) {
		if (mNodes[n].parent == pParentNode) {
			childNodes.push_back(n);
		};
	};

	if (childNodes.size() == 0) {
		if (pParentNode == -1) {
			// nothing???
		} else {
			// cap our parent slice
			capSlice(pParentSlice);

			// and add our leaves
			vec3	tangent	= mVertices[mNodes[pParentNode].b] - mVertices[mNodes[pParentNode].a];
			tangent = tangent.normalized();
			vec3	bitangent = tangent * mNormals[pParentSlice.p[0]];
			bitangent = bitangent.normalized();
			
			addLeaves(mVertices[mNodes[pParentNode].a] + pOffset, tangent, bitangent);
			addLeaves(mVertices[mNodes[pParentNode].a] + pOffset, tangent, bitangent * -1.0f);
		};
	} else if (childNodes.size() == 1) {
		// we just need to create a slice at our root
		int		node		= childNodes[0];
		float	size		= mNodes[node].childcount;
		size = (size * mRadiusFactor) + mMinRadius;
		vec3	direction	= mVertices[mNodes[node].b] - mVertices[mNodes[node].a];
		float	len			= direction.length();
		direction /= len;
		if (pParentNode != -1) {
			vec3	parentDir	= mVertices[mNodes[pParentNode].b] - mVertices[mNodes[pParentNode].a];
			direction += parentDir.normalized();
			direction = direction.normalized();
		};
		
		vec3	bitangent = mNormals[pParentSlice.p[3]];
		slice	childSlice	= createSlice(mVertices[mNodes[node].a] + pOffset, direction, bitangent, size, pDistance);

		if (pParentNode != -1) {
			// join parent to child
			joinTwoSlices(pParentSlice, childSlice);
		};

		expandChildren(node, childSlice, pOffset, pDistance + len);
	} else {		
		int		firstChild	= (pParentNode == -1 ? 0 : 1);
		int		numSlices	= childNodes.size() + firstChild;
		slice*	slices		= new slice[numSlices];
		vec3	bitangent	= vec3(1.0f, 0.0f, 0.0f);
		
		if (pParentNode != -1) {
			// draw our tree up to the point of our split
			
			float	size		= mNodes[pParentNode].childcount;
			size = (size * mRadiusFactor) + mMinRadius;
			vec3	direction	= mVertices[mNodes[pParentNode].b] - mVertices[mNodes[pParentNode].a];
			float	len			= direction.length();
			direction /= len;

			bitangent = mNormals[pParentSlice.p[3]];
			slices[0] = createSlice(mVertices[mNodes[pParentNode].b] + pOffset, direction, bitangent, size, pDistance);
			bitangent = mNormals[slices[0].p[3]];

			// join final piece
			joinTwoSlices(pParentSlice, slices[0]);
			
			// now add in some room..
			pOffset += direction * size;
			pDistance += size;
		};	
		
		for (int n = 0; n < childNodes.size(); n++) {
			int		node		= childNodes[n];
			float	size		= mNodes[node].childcount;
			size = (size * mRadiusFactor) + mMinRadius;
			vec3	direction	= mVertices[mNodes[node].b] - mVertices[mNodes[node].a];
			float	len			= direction.length();
			direction /= len;
			if (pParentNode != -1) {
				vec3	parentDir	= mVertices[mNodes[pParentNode].b] - mVertices[mNodes[pParentNode].a];
				direction += parentDir.normalized();
				direction = direction.normalized();
			};
			vec3	offset		= direction * size;

			slices[n + firstChild] = createSlice(mVertices[mNodes[node].a] + pOffset + offset, direction, bitangent, size, pDistance + size);

			expandChildren(node, slices[n + firstChild], pOffset + offset, pDistance + size + len);
		};
		
		// now create joining piece
		joinMultiSlices(numSlices, slices);
		
		delete slices;		
	};
	
};

/**
 * createModel()
 * 
 * This method will use our node tree to build a model of our tree
 *
 **/
void treelogic::createModel() {
	unsigned long	vertCount	= mVertices.size(); // remember how many vertices we have right now so we can remove these later on...

	slice emptySlize;
	expandChildren(-1, emptySlize, vec3(0.0f, 0.0f, 0.0f), 0.0f);
	
	// now remove our nodes and related vertices, we no longer need them...
	for (unsigned long i = 0; i < vertCount; i++) {
		remVertex(0);
	};
	mNodes.clear();
};


/////////////////////////////////////////////////////////////////////
// shaders
//
// Normally you would have your shader files on disk, load them up
// and compile then. Here we're just compiling the programs into our
// executable. This is mostly just to simplify any x-platform issues
// that would just cloud the example code.
//
// For simple shaders like this that is fine but it makes them
// slightly harder to read. 
//
/////////////////////////////////////////////////////////////////////

void treelogic::initShaders() {
	makeSimpleShader();
	makeTreeShader();
	makeLeafShader();
};

void treelogic::makeSimpleShader() {
	if (mSimpleShader == NULL) {
		// create a new shader
		mSimpleShader = new shader();
		
		syslog(LOG_NOTICE, "Creating simple shader");

		mSimpleShader->addShader(GL_VERTEX_SHADER, shader::loadShaderText("simpleshader.vs").c_str());
		mSimpleShader->addShader(GL_FRAGMENT_SHADER, shader::loadShaderText("simpleshader.fs").c_str());
		mSimpleShader->link();
	};
};

void treelogic::makeTreeShader() {
	if (mTreeShader == NULL) {
		// create a new shader
		mTreeShader = new shader();
		
		syslog(LOG_NOTICE, "Creating tree shader");

		mTreeShader->addShader(GL_VERTEX_SHADER, shader::loadShaderText("treeshader.vs").c_str());
		mTreeShader->addShader(GL_TESS_CONTROL_SHADER, shader::loadShaderText("treeshader.ts").c_str());
		mTreeShader->addShader(GL_TESS_EVALUATION_SHADER, shader::loadShaderText("treeshader.te").c_str());
		mTreeShader->addShader(GL_FRAGMENT_SHADER, shader::loadShaderText("treeshader.fs").c_str());
		mTreeShader->link();
	};
};

void treelogic::makeLeafShader() {
	if (mLeafShader == NULL) {
		// create a new shader
		mLeafShader = new shader();
		
		syslog(LOG_NOTICE, "Creating leaf shader");

		mLeafShader->addShader(GL_VERTEX_SHADER, shader::loadShaderText("leafshader.vs").c_str());
		mLeafShader->addShader(GL_FRAGMENT_SHADER, shader::loadShaderText("leafshader.fs").c_str());
		mLeafShader->link();
	};
};

/////////////////////////////////////////////////////////////////////
// rendering
/////////////////////////////////////////////////////////////////////

/**
 * render()
 * 
 * This method will render our tree information to screen
 *
 **/
void treelogic::render() {
	unsigned long i;
	unsigned long numOfVerts = mVertices.size();
	
	// OpenGL 3 requires us to have a vertex array buffer and store our data in vertex buffers. 
	// These allow you to bind and set your buffers once and just enable the vertex array buffer to select them.
	// Normally after loading a model into memory you would do this once way before your start your render loop. 
	// We do it here in our render loop for convinience and because we're changing our data regularly.
	
	// We also are required to use shader programs which you can find up above.
	
	// create it the first time...
	if (mVAO_Tree == 0) {
		glGenVertexArrays(1, &mVAO_Tree);
	};
	
	// now bind it
	glBindVertexArray(mVAO_Tree);
	
	// now load our buffers if we must
	if (mUpdateBuffers && (numOfVerts > 0)) {		
		// create a buffer for our vertices
		if (mVBO_Verts == 0) {
			// create our buffer
			glGenBuffers(1, &mVBO_Verts);
		};

		// bind our buffer
		glBindBuffer(GL_ARRAY_BUFFER, mVBO_Verts);

		// allocate our buffer (but no data yet), we're initially lying about static draw but that should be ok...
		glBufferData(GL_ARRAY_BUFFER, (sizeof(vec3) + sizeof(vec3) + sizeof(vec2)) * numOfVerts, NULL, GL_STATIC_DRAW);			
		
		// copy our positions
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * numOfVerts, mVertices.data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid *) 0);
		
		// copy our normals
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3) * numOfVerts, sizeof(vec3) * numOfVerts, mNormals.data());
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid *) (sizeof(vec3) * numOfVerts));
		
		// copy our texture coords
		glBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(vec3) * numOfVerts, sizeof(vec2) * numOfVerts, mTexCoords.data());
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (GLvoid *) (2 * sizeof(vec3) * numOfVerts));
		
		// and setup our elements buffer
		if (mVBO_TreeElements == 0) {
			// create our buffer
			glGenBuffers(1, &mVBO_TreeElements);
		};
		
		// bind our buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBO_TreeElements);	
		if (mTreeElements.size() > 0) {
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 4 * mTreeElements.size(), mTreeElements.data(), GL_STATIC_DRAW);			
		} else if (mNodes.size() > 0) {
			// Our nodes contain way to much data, but this time we'll need to copy
			unsigned long numOfNodes = mNodes.size();
			GLuint* nodes = new GLuint[numOfNodes*2];
			
			for (unsigned long n = 0; n < numOfNodes; n++) {
				nodes[n*2] = mNodes[n].a;
				nodes[n*2+1] = mNodes[n].b;
			};
			
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 2 * numOfNodes, nodes, GL_STATIC_DRAW);			
			
			delete nodes;
		};
		
		mUpdateBuffers = false;
	};
	
	// if we have our elements, start there..
	if (mTreeElements.size() > 0) {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// wireframe
		if (mWireFrame) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);			
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);			
		};
		
		// setup our texture
		glActiveTexture(GL_TEXTURE0);
		
		if (mBarkTextID == 0) {
			mBarkTextID = shader::loadTexture("tree.jpg");
		} else {
			// bind our texture
			glBindTexture(GL_TEXTURE_2D, mBarkTextID);			
		};
		
		// use our tree shader program
		glUseProgram(mTreeShader->shaderProgram());
		
		// set our projection/model/view matrix
		mTreeShader->setIntUniform(mTreeShader->uniform("treeTexture"), 0);
		mTreeShader->setMat4Uniform(mTreeShader->uniform("mvp"), mProjection * mView * mModel);
		mTreeShader->setMat3Uniform(mTreeShader->uniform("normalMat"), mModel.mat3x3());

		// in OpenGL we render these as patches and it goes through our tesselation shader
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glDrawElements(GL_PATCHES, mTreeElements.size() * 4, GL_UNSIGNED_INT, 0);
		
		/* now its time for our leaves */
		if (mLeafElements.size() > 0) {
			// create our VAO
			if (mVAO_Leaves ==0) {
				glGenVertexArrays(1, &mVAO_Leaves);
			};
			
			// now bind it
			glBindVertexArray(mVAO_Leaves);
			
			// create and load our buffers if we must
			if (mVBO_LeafElements == 0) {
				// our vertex buffer is loaded and should be unchanged or we wouldn't be here, reuse it..
				glBindBuffer(GL_ARRAY_BUFFER, mVBO_Verts);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid *) 0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid *) (sizeof(vec3) * numOfVerts));
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (GLvoid *) (2 * sizeof(vec3) * numOfVerts));
				
				// create our VBO for our leaf elements
				glGenBuffers(1, &mVBO_LeafElements);
				
				// bind it
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBO_LeafElements);	
				
				// and load our data
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * mLeafElements.size(), mLeafElements.data(), GL_STATIC_DRAW);			
			};

			// setup our texture, texture 0 should still be the active texture
			if (mLeafTextID == 0) {
				mLeafTextID = shader::loadTexture("leaves_3.png");
			} else {
				// bind our texture
				glBindTexture(GL_TEXTURE_2D, mLeafTextID);			
			};
			
			// setup our leaf shader
			glUseProgram(mLeafShader->shaderProgram());
		
			// set our projection/model/view matrix
			mLeafShader->setIntUniform(mLeafShader->uniform("leafTexture"), 0);
			mLeafShader->setMat4Uniform(mLeafShader->uniform("mvp"), mProjection * mView * mModel);
			mLeafShader->setMat3Uniform(mLeafShader->uniform("normalMat"), mModel.mat3x3());
			
			// and draw...
			glDrawElements(GL_TRIANGLES, mLeafElements.size() * 3, GL_UNSIGNED_INT, 0);
		};
		
		// back to normal..
		if (mWireFrame) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		};
		
		// and unbind our texture
		glBindTexture(GL_TEXTURE_2D, 0);		
	} else {
		// ignore depth and culling
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		
		// use our simple shader program
		glUseProgram(mSimpleShader->shaderProgram());
		GLint colorID = mSimpleShader->uniform("color");
		
		// set our projection/model/view matrix
		mSimpleShader->setMat4Uniform(mSimpleShader->uniform("mvp"), mProjection * mView * mModel);
		
		// draw our (remaining) attraction points
		if (mAttractionPoints.size() > 0) {
			// create our own VAO for our APoints as also our vertex attribut array settings are bound to this..
			if (mVAO_APoints == 0) {
				glGenVertexArrays(1, &mVAO_APoints);
			};
			
			// Bind our vertex array
			glBindVertexArray(mVAO_APoints);
			
			// make sure we have a vertex buffer
			if (mVBO_APoints == 0) {
				glGenBuffers(1, &mVBO_APoints);
			};
			
			// Bind our vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, mVBO_APoints);
			
			// always reload our points, note that our position is at the start of our class so we can load it as is and just ignore the additional data
			glBufferData(GL_ARRAY_BUFFER, sizeof(attractionPoint) * mAttractionPoints.size(), mAttractionPoints.data(), GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(attractionPoint), (GLvoid *) 0);
			
			// set our color
			mSimpleShader->setVec4Uniform(colorID, vec4(0.0f, 1.0f, 0.0f, 1.0f)); // green
			
			// and draw!
			glPointSize(2.0f);
			glDrawArrays(GL_POINTS, 0, mAttractionPoints.size()); // draw our points
			
			// bind our trees VAO again
			glBindVertexArray(mVAO_Tree);
		} else {
			if (mVBO_APoints != 0) {
				// we are done with this so get rid of it...
				glDeleteBuffers(1, &mVBO_APoints);
				mVBO_APoints = 0;
			};
			if (mVAO_APoints != 0) {
				glDeleteVertexArrays(1, &mVAO_APoints);
				mVAO_APoints = 0;
			};
		};
		
		// draw points for our vertices to highlight them
		if (mVertices.size() > 0) {
			// set our color
			mSimpleShader->setVec4Uniform(colorID, vec4(170.0f / 256.0f, 80.0f / 256.0f, 0.0f, 1.0f)); // brown
			
			// and draw!
			glPointSize(3.0f);
			glDrawArrays(GL_POINTS, 0, mVertices.size()); // draw our points
		};
		
		// draw the tree as far as we've build it...
		if (mNodes.size() > 0) {
			mSimpleShader->setVec4Uniform(colorID, vec4(204.0f / 256.0f, 102.0f / 256.0f, 0.0f, 1.0f)); // brown
			
			glDrawElements(GL_LINES, mNodes.size() * 2, GL_UNSIGNED_INT, 0);
		};
			
		// reset our point size
		glPointSize(1.0f);	
	};
	
	// reset our shader
	glUseProgram(0);
	
	// reset our vertex array
	glBindVertexArray(0);
};

#endif
