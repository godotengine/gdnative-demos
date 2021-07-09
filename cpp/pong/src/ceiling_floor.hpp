#ifndef CEILING_FLOOR_H
#define CEILING_FLOOR_H

#include <Area2D.hpp>
#include <Godot.hpp>

#include "ball.hpp"

class CeilingFloor : public godot::Area2D {
	GODOT_CLASS(CeilingFloor, godot::Area2D)

public:
	int bounce_direction = 1;

	void _init() {}
	void _on_area_entered(Ball *p_ball);

	static void _register_methods();
};

#endif // CEILING_FLOOR_H
