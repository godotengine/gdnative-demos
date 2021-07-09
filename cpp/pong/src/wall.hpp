#ifndef WALL_H
#define WALL_H

#include <Area2D.hpp>
#include <Godot.hpp>

#include "ball.hpp"

class Wall : public godot::Area2D {
	GODOT_CLASS(Wall, godot::Area2D)

public:
	void _init() {}
	void _on_wall_area_entered(Ball *p_area);

	static void _register_methods();
};

#endif // WALL_H
