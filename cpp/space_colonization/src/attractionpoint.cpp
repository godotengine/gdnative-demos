/********************************************************************
 * class for our attraction point
 * 
 * Based on my original implementation here
 * https://github.com/BastiaanOlij/Trees
 * 
 * By Bastiaan Olij
********************************************************************/

#include "attractionpoint.h"

AttractionPoint::AttractionPoint() {
	x = 0;
	y = 0;
	z = 0;
	closest_vertice = 0;
};

AttractionPoint::AttractionPoint(float p_x, float p_y, float p_z) {
	x = p_x;
	y = p_y;
	z = p_z;
	closest_vertice = 0;
};

AttractionPoint::AttractionPoint(const AttractionPoint &p_copy) {
	x = p_copy.x;
	y = p_copy.y;
	z = p_copy.z;
	closest_vertice = p_copy.closest_vertice;
};

AttractionPoint &AttractionPoint::operator=(const AttractionPoint &p_copy) {
	x = p_copy.x;
	y = p_copy.y;
	z = p_copy.z;
	closest_vertice = p_copy.closest_vertice;
	return (*this);
};
