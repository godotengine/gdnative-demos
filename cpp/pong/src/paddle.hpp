#ifndef PADDLE_H
#define PADDLE_H

#include <Area2D.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <RandomNumberGenerator.hpp>

#include "ball.hpp"

class Paddle : public godot::Area2D {
	GODOT_CLASS(Paddle, godot::Area2D)

	const int MOVE_SPEED = 100;

	int _ball_dir;
	godot::String _up;
	godot::String _down;
	godot::Input *_input;
	godot::Ref<godot::RandomNumberGenerator> _random;

	real_t _screen_size_y;

public:
	void _init() {}
	void _ready();
	void _process(const double p_delta);
	void _on_area_entered(Ball *p_area);

	static void _register_methods();
};

#endif // PADDLE_H
