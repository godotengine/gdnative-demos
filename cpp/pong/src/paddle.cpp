#include "paddle.hpp"

void Paddle::_ready() {
	_input = godot::Input::get_singleton();
	_random = (godot::Ref<godot::RandomNumberGenerator>)godot::RandomNumberGenerator::_new();
	_random->randomize();
	_screen_size_y = get_viewport_rect().size.y;
	godot::String n = godot::String(get_name()).to_lower();
	_up = n + "_move_up";
	_down = n + "_move_down";
	if (n == "left") {
		_ball_dir = 1;
	} else {
		_ball_dir = -1;
	}
}

void Paddle::_process(const double delta) {
	// Move up and down based on input.
	real_t keyboard_input = _input->get_action_strength(_down) - _input->get_action_strength(_up);
	godot::Vector2 position = get_position();
	position.y = (real_t)godot::Math::clamp(position.y + keyboard_input * MOVE_SPEED * delta, 16.0, _screen_size_y - 16.0);
	set_position(position);
}

void Paddle::_on_area_entered(Ball *p_ball) {
	if (p_ball->get_name() == "Ball") {
		// Assign new direction.
		p_ball->direction = godot::Vector2((real_t)_ball_dir, _random->randf() * 2 - 1).normalized();
	}
}

void Paddle::_register_methods() {
	godot::register_method("_ready", &Paddle::_ready);
	godot::register_method("_process", &Paddle::_process);
	godot::register_method("_on_area_entered", &Paddle::_on_area_entered);
}
