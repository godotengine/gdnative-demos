#ifndef BALL_H
#define BALL_H

#include <Area2D.hpp>
#include <Godot.hpp>

class Ball : public godot::Area2D {
	GODOT_CLASS(Ball, godot::Area2D)

	const int DEFAULT_SPEED = 100;
	real_t _speed = DEFAULT_SPEED;
	godot::Vector2 _initial_pos;

public:
	godot::Vector2 direction = godot::Vector2(-1, 0);

	void _init() {}
	void _ready();
	void _process(double p_delta);
	void reset();

	static void _register_methods();
};

#endif // BALL_H
