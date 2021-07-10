#include "wall.hpp"

void Wall::_on_wall_area_entered(Ball *p_ball) {
	if (p_ball->get_name() == "Ball") {
		// Ball went out of game area, reset.
		p_ball->reset();
	}
}

void Wall::_register_methods() {
	godot::register_method("_on_wall_area_entered", &Wall::_on_wall_area_entered);
}
