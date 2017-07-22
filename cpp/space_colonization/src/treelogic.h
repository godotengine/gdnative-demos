/********************************************************************
 * treelogic contains the logic to generate our tree
 * 
 * Implementation based on the "Space Colonization Algorithm" by
 * Adam Runions, Brendan Lane, and Przemyslaw Prusinkiewicz
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#ifdef NOTDEFINED

#ifndef treelogich
#define treelogich

#define		GLFW_INCLUDE_GL_3
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

// standard libraries we need...
#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include <vector>

#include "vec2.h"
#include "vec3.h"
#include "mat4.h"
#include "vec4.h"
#include "shader.h"

#include "attractionpoint.h"
#include "treenode.h"

// class for a slice
class slice {
public:
	unsigned long p[5];											// 5 vertices to a slice
	
	slice();
	slice(const slice& pCopy);

	slice& operator=(const slice& pCopy);
};

// class for a triangle
class triangle {
public:
	GLuint v[3];
	
	triangle();
	triangle(const triangle& pCopy);
	
	triangle& operator=(const triangle& pCopy);
};

// class for a quad
class quad {
public:
	GLuint v[4];												// index to our vertex buffer
	
	quad();
	quad(const quad& pCopy);
	
	quad& operator=(const quad& pCopy); 
};

// Our treelogic class, note that after we are finished only mVertices, mNormals, mTexCoords and mElements are relevant
class treelogic {
private:
	std::vector<attractionPoint>		mAttractionPoints;		// our attraction points
	std::vector<vec3>					mVertices;				// vertices that make up our tree
	std::vector<vec3>					mNormals;				// normals for our vertice
	std::vector<vec2>					mTexCoords;				// texture coordinates
	std::vector<treenode>				mNodes;					// nodes used to construct our tree skeleton
	std::vector<slice>					mSlices;				// slices that form the basis of
	std::vector<quad>					mTreeElements;			// our tree elements
	std::vector<triangle>				mLeafElements;			// our leaf elements
	
	unsigned long						mLastNumOfVerts;		// number of vertices before we added our last round of nodes
	
	bool								mWireFrame;				// if true we render our wireframe
	mat4								mProjection;			// our projection matrix
	mat4								mView;					// our view matrix
	mat4								mModel;					// our model matrix
	shader*								mSimpleShader;			// simple shader to render our points
	shader*								mTreeShader;			// shader we use to render our tree
	shader*								mLeafShader;			// shader we use to render our leafs
	
	GLuint								mVAO_APoints;			// Vertex array for our attraction points
	GLuint								mVBO_APoints;			// Vertex buffer for our attraction points
	
	bool								mUpdateBuffers;			// Do we need to update our vertex buffer?
	GLuint								mVAO_Tree;				// Our vertex array buffer for our tree
	GLuint								mVAO_Leaves;			// our leaves array buffer
	GLuint								mVBO_Verts;				// Vertex buffer for our vertexs
	GLuint								mVBO_TreeElements;		// Vertex buffer for our tree elements
	GLuint								mVBO_LeafElements;		// Vertex buffer for our leaf elements
	
	GLuint								mBarkTextID;			// ID of our bark texture map
	GLuint								mLeafTextID;			// ID of our leaf texture map
	
	float								mMinRadius;				// Minimum radius for our tree
	float								mRadiusFactor;			// Factor to apply to calculate the radius of our tree
	vec2								mLeafSize;				// Size of our leaf	

	float randf(float pMin = -1.0f, float pMax = 1.0f);
	unsigned long addVertex(const vec3& pVertex);
	void remVertex(unsigned long pIndex);
	
	slice createSlice(vec3 pCenter, vec3 pPlaneNormal, vec3 pBitangent, float pSize, float pDistance);
	void capSlice(const slice& pSlice);
	void joinTwoSlices(const slice& pA, const slice& pB);
	void joinMultiSlices(long pSliceCount, slice* pSlices);
	void addLeaves(vec3 pCenter, vec3 pTangent, vec3 pBiTangent);
	void expandChildren(unsigned long pParentNode, const slice& pParentSlice, vec3 pOffset, float pDistance);

	void makeSimpleShader();
	void makeTreeShader();
	void makeLeafShader();

protected:
public:	
	// constructors/destructors
	treelogic();
	~treelogic();
	
	// properties
	bool wireframe();
	void setWireframe(bool pWireframe);
	float minRadius();
	void setMinRadius(float pRadius);
	float radiusFactor();
	void setRadiusFactor(float pFactor);
	
	// matrixes
	mat4 projection();
	void setProjection(const mat4& pProjection);
	mat4 view();
	void setView(const mat4& pView);
	mat4 model();
	void setModel(const mat4& pModel);
	
	// tree generation code
	unsigned long growBranch(unsigned long pFromVertex, vec3 pTo);
	void generateAttractionPoints(unsigned long pNumOfPoints = 5000, float pOuterRadius = 100.0f, float pInnerRadius = 50.0f, float pAspect = 3.0f, float pOffsetY = 20.0f, bool pClear = true);
	bool doIteration(float pMaxDistance = 75.0f, float pBranchSize = 5.0f, float pCutOffDistance = 10.0f, vec3 pBias = vec3(0.0, 0.0, 0.0));
	void optimiseNodes();
	void createModel();
	
	// shaders
	void initShaders();
	
	// rendering
	void render();
};

#endif

#endif
