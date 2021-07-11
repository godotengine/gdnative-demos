#include "ball.hpp"

void Ball::_ready() {
	_initial_pos = get_position();
}

void Ball::_process(const double p_delta) {
	_speed += (real_t)p_delta * 2;
	godot::Vector2 position = get_position();
	position += _speed * (real_t)p_delta * direction;
	set_position(position);
}

void Ball::reset() {
	direction = godot::Vector2(-1, 0);
	set_position(_initial_pos);
	_speed = DEFAULT_SPEED;
}

void Ball::_register_methods() {
	godot::register_method("_ready", &Ball::_ready);
	godot::register_method("_process", &Ball::_process);
	godot::register_method("reset", &Ball::reset);
}
