#ifndef Gen_Texture_H
#define Gen_Texture_H

#include <Godot.hpp>
#include <ImageTexture.hpp>
#include "open-simplex-noise.h"

namespace godot {

class Gen_Texture : public godot::GodotScript<ImageTexture> {
	GODOT_CLASS(Gen_Texture)

private:
	typedef struct point {
		float x, y, z;
	} point;

	typedef struct worley_cloud {
		int number_of_points;
		float max_distance;
		point * points;
	} worley_cloud;

	unsigned int seed;
	struct osn_context *osn_ctx;

	float width;
	float height;
	float depth;
	worley_cloud cloud;

	PoolByteArray image_data;

	float random();
	float clamp(float p_value, float p_min, float p_max);
	int clamp(int p_value, int p_min, int p_max);
	void make_worley_pointcloud(int p_number_of_points);
	void free_worley_pointcloud();
	float get_worley_min_distance(point pos, float octave = 1.0);

public:
	static void _register_methods();

	Gen_Texture();
	~Gen_Texture();

	void set_seed(unsigned int p_seed);
	unsigned int get_seed();

	void set_texture_size(int p_width, int p_height, int p_depth);
	void generate_worley_point_cloud(int p_number_of_points);
	void worley_texture(float amplitude, float octave, int channel, bool add);
	void simplex_texture(float amplitude, float octave, int channel, bool add);
	void bake_texture();
};

}

#endif /* !Gen_Texture_H */
