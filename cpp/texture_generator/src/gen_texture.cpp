#include "gen_texture.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace godot;

/****************************************************************************************************
 * public functions
 ****************************************************************************************************/

void Gen_Texture::_register_methods() {
	// @TODO add seed as property
	register_method((char *)"set_seed", &Gen_Texture::set_seed);
	register_method((char *)"get_seed", &Gen_Texture::get_seed);

	register_method((char *)"set_texture_size", &Gen_Texture::set_texture_size);
	register_method((char *)"generate_worley_point_cloud", &Gen_Texture::generate_worley_point_cloud);
	register_method((char *)"worley_texture", &Gen_Texture::worley_texture);
	register_method((char *)"simplex_texture", &Gen_Texture::simplex_texture);
	register_method((char *)"bake_texture", &Gen_Texture::bake_texture);
}

Gen_Texture::Gen_Texture() {
	width = 0;
	height = 0;
	depth = 0;
	osn_ctx	= NULL;
	cloud.number_of_points = 0;
	cloud.points = NULL;

	set_seed(time(NULL));
}

Gen_Texture::~Gen_Texture() {
	free_worley_pointcloud();

	if (osn_ctx != NULL) {
		open_simplex_noise_free(osn_ctx);
		osn_ctx = NULL;
	}
}

/*
 * Random seed to use
*/
void Gen_Texture::set_seed(unsigned int p_seed) {
	seed = p_seed;
	srand(seed);

	// (re)init our simplex noise
	if (osn_ctx != NULL) {
		open_simplex_noise_free(osn_ctx);
		osn_ctx = NULL;
	}
	open_simplex_noise(seed, &osn_ctx);
}

unsigned int Gen_Texture::get_seed() {
	return seed;
}

/*
 * Note that Godot currently does not support 3D textures so we're going to place the slices one after the other.
 * Once proper support for 3D textures is added we'll switch over.
 * Also note that we have 4 colour channels, so we can pack 4 3D textures in one :P
*/
void Gen_Texture::set_texture_size(int p_width, int p_height, int p_depth) {
	int image_size = p_width * p_height * p_depth * 4;

	if (image_size == 0) {
		printf("Image can't be empty!\n");
		return;
	}
	image_data.resize(image_size);

	// make sure we fill our buffers with zeroes
	PoolByteArray::Write image_write = image_data.write();
	memset(image_write.ptr(), 0, image_size);

	// remember our sizes
	width = p_width;
	height = p_height;
	depth = p_depth;
}

void Gen_Texture::generate_worley_point_cloud(int p_number_of_points) {
	// if we already have a point cloud, free it
	free_worley_pointcloud();

	// and now create our new point cloud
	make_worley_pointcloud(p_number_of_points);
}

void Gen_Texture::worley_texture(float amplitude, float octave, int channel, bool add) {
	if ((width == 0) || (height == 0) || (depth == 0)) {
		printf("No texture data to generate texture into\n");
		return;
	}

	if ((channel < 0) || (channel > 3)) {
		printf("Invalid colour channel, choose 0 through 3!\n");
		return;		
	}

	printf("Worley texture, amp %f, octave %f, channel %i\n", amplitude, octave, channel);

	// lock memory and give us direct access
	PoolByteArray::Write image_write = image_data.write();
	unsigned char * image_ptr = image_write.ptr();
	point pos, step;

	pos.z = 0.5 / depth;
	step.x = 1.0 / width;
	step.y = 1.0 / height;
	step.z = 1.0 / depth;

	image_ptr += channel;

	for (int z = 0; z < depth; z++) {
		pos.y = 0.5 / height;
		for (int y = 0; y < height; y++) {
			pos.x = 0.5 / width;
			for (int x = 0; x < width; x++) {
				// Octave 1
				float dist = amplitude * get_worley_min_distance(pos, octave) / cloud.max_distance;
				int value = floor(255.0 * dist);
				if (add)
					value += *image_ptr;
				*image_ptr = clamp(value, 0, 255);

				image_ptr+= 4;
				pos.x += step.x;
			}

			pos.y += step.y;
		}

		pos.z += step.z;
	}
}

void Gen_Texture::simplex_texture(float amplitude, float octave, int channel, bool add) {
	if ((width == 0) || (height == 0) || (depth == 0)) {
		printf("No texture data to generate texture into\n");
		return;
	}

	if ((channel < 0) || (channel > 3)) {
		printf("Invalid colour channel, choose 0 through 3!\n");
		return;		
	}

	printf("Simplex texture, amp %f, octave %f, channel %i\n", amplitude, octave, channel);

	// lock memory and give us direct access
	PoolByteArray::Write image_write = image_data.write();
	unsigned char * image_ptr = image_write.ptr();
	point pos, step;

	pos.z = 0.5 / depth;
	step.x = 1.0 / width;
	step.y = 1.0 / height;
	step.z = 1.0 / depth;

	image_ptr += channel;

	// we upscale our octave. probably need to improve tile-ability of this.
	octave *= 32.0;

	for (int z = 0; z < depth; z++) {
		pos.y = 0.5 / height;
		for (int y = 0; y < height; y++) {
			pos.x = 0.5 / width;
			for (int x = 0; x < width; x++) {
				// Octave 1
				float dist = amplitude * open_simplex_noise3(osn_ctx, octave * pos.x, octave * pos.y, octave * pos.z);
				int value = floor(255.0 * dist);
				if (add)
					value += *image_ptr;
				*image_ptr = clamp(value, 0, 255);

				image_ptr+= 4;
				pos.x += step.x;
			}

			pos.y += step.y;
		}

		pos.z += step.z;
	}	
}


/*
 * Take our image buffer and load it up as our texture data!
 */
void Gen_Texture::bake_texture() {
	if ((width == 0) || (height == 0) || (depth == 0)) {
		printf("No texture data to bake\n");
		return;
	}

	// make our texture, note, eventually we'll switch this between 2D and 3D depending on p_depth
	Ref<Image> img;
	img.instance();
	img->create_from_data(width, height * depth, false, 5, image_data); // 5 = FORMAT_RGBA8

	owner->create_from_image(img, 2 + 4); // 2 = repeat, 4 = filter
	img.unref();
}

/****************************************************************************************************
 * private functions
 ****************************************************************************************************/

/*
 * simple function returning a random value between 0.0 and 1.0
*/
float Gen_Texture::random() {
	float random_f = rand(); // rand returns an int between 0 and RAND_MAX, convert to float
	float max_f = RAND_MAX;

	return random_f / max_f;
}

/*
 * simple functions to keep a value between two other values
*/
float Gen_Texture::clamp(float p_value, float p_min, float p_max) {
	if (p_value < p_min) {
		return p_min;
	} else if (p_value > p_max) {
		return p_max;
	} else {
		return p_value;
	}
}

int Gen_Texture::clamp(int p_value, int p_min, int p_max) {
	if (p_value < p_min) {
		return p_min;
	} else if (p_value > p_max) {
		return p_max;
	} else {
		return p_value;
	}
}

/*
 * worley noise works by creating a set of random generated points to which we calculate the distance
 * note that we create 27 copies of each point to make the textures we generate tileable
*/
void Gen_Texture::make_worley_pointcloud(int p_number_of_points) {
	int index = 0;

	cloud.number_of_points = p_number_of_points * 27;
	cloud.points = (point *)malloc(sizeof(point) * cloud.number_of_points);

	for (int i = 0; i < p_number_of_points; i++) {
		// random generate a point
		point p;
		p.x = random();
		p.y = random();
		p.z = random();

		// add this including adjoining sectors
		for (float z = -1.0; z < 1.5; z += 1.0) { // -1.0, 0.0, 1.0
			for (float y = -1.0; y < 1.5; y += 1.0) { // -1.0, 0.0, 1.0
				for (float x = -1.0; x < 1.5; x += 1.0) { // -1.0, 0.0, 1.0
					cloud.points[index].x = p.x + x;
					cloud.points[index].y = p.y + y;
					cloud.points[index].z = p.z + z;

					// printf("Point %i = %f, %f, %f\n", index, cloud.points[index].x, cloud.points[index].y, cloud.points[index].z);

					index++;
				}
			}
		}
	}

	// calculate our maximum, minimum distance so we can normalise our results :)
	float max_dist_squared = 0.0;

	for (int j = 0; j < cloud.number_of_points; j++) {
		float min_dist_squared = 100.0; 

		for (int i = 0; i < cloud.number_of_points; i++) {
			if (i != j) {
				point delta;
				delta.x = cloud.points[i].x - cloud.points[j].x;
				delta.y = cloud.points[i].y - cloud.points[j].y;
				delta.z = cloud.points[i].z - cloud.points[j].z;

				float distance_squared = (delta.x * delta.x) + (delta.y * delta.y) + (delta.z * delta.z);
				// printf("Checking %i, %i => %f, %f, %f => %f\n", i, j, delta.x, delta.y, delta.z, distance_squared);

				if (distance_squared < min_dist_squared) {
					min_dist_squared = distance_squared;
				}				
			}
		}

		if (min_dist_squared > max_dist_squared) {
			max_dist_squared = min_dist_squared;
		}
	}

	// save the sqrt for last :)
	cloud.max_distance = sqrt(max_dist_squared);
}

void Gen_Texture::free_worley_pointcloud() {
	cloud.number_of_points = 0;
	if (cloud.points != NULL) {
		// and free up stuff we no longer need
		free(cloud.points);
		cloud.points = NULL;
	}
}

/*
 * get the closest distance in our worley cloud
*/
float Gen_Texture::get_worley_min_distance(point pos, float octave) {
	float min_dist_squared = 100.0;

	// make sure our point is tiled
	pos.x = fmod(pos.x * octave, 1.0);
	pos.x = pos.x < 0.0 ? pos.x + 1.0 : pos.x;
	pos.y = fmod(pos.y * octave, 1.0);
	pos.y = pos.y < 0.0 ? pos.y + 1.0 : pos.y;
	pos.z = fmod(pos.z * octave, 1.0);
	pos.z = pos.z < 0.0 ? pos.z + 1.0 : pos.z;

	for (int i = 0; i < cloud.number_of_points; i++) {
		point delta;
		delta.x = cloud.points[i].x - pos.x;
		delta.y = cloud.points[i].y - pos.y;
		delta.z = cloud.points[i].z - pos.z;

		float distance_squared = (delta.x * delta.x) + (delta.y * delta.y) + (delta.z * delta.z);
		if (distance_squared < min_dist_squared) {
			min_dist_squared = distance_squared;
		}
	}

	return sqrt(min_dist_squared);
}
