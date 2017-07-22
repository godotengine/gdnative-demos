/********************************************************************
 * class for our attraction point
 * 
 * Based on my original implementation here
 * https://github.com/BastiaanOlij/Trees
 * 
 * By Bastiaan Olij
********************************************************************/

#ifndef ATTRACTION_POINT_H
#define ATTRACTION_POINT_H

class AttractionPoint {
public:
	float x, y, z;
	unsigned long closest_vertice;

	AttractionPoint();
	AttractionPoint(float p_x, float p_y, float p_z);
	AttractionPoint(const AttractionPoint &p_copy);

	AttractionPoint &operator=(const AttractionPoint &p_copy);
};

#endif
