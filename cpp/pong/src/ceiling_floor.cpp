#include "ceiling_floor.hpp"

void CeilingFloor::_on_area_entered(Ball *p_ball) {
	if (p_ball->get_name() == "Ball") {
		p_ball->direction = (p_ball->direction + godot::Vector2(0, (real_t)bounce_direction)).normalized();
	}
}

void CeilingFloor::_register_methods() {
	godot::register_method("_on_area_entered", &CeilingFloor::_on_area_entered);
	godot::register_property("bounce_direction", &CeilingFloor::bounce_direction, 1);
}
