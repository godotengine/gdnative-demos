#include "Simple.hpp"

#include "Input.hpp"

void Simple::_register_methods() {
	godot::register_method("get_data", &Simple::get_data);
}

void Simple::_init() {
	data = "Hello World from C++";
}

godot::String Simple::get_data() const {
	return data;
}

void SimpleSprite::_register_methods() {
	godot::register_method("_process", &SimpleSprite::_process);
}

// `_init` doesn't need to be registered in `_register_methods`.
void SimpleSprite::_init() {
	godot::Godot::print("A SimpleSprite was initialized in GDNative!");
}

void SimpleSprite::_process(double delta) {
	godot::Input *input = godot::Input::get_singleton();
	godot::Vector2 input_dir(0, 0);

	input_dir.x = input->get_action_strength("ui_right") - input->get_action_strength("ui_left");
	input_dir.y = input->get_action_strength("ui_down") - input->get_action_strength("ui_up");

	set_position(get_position() + input_dir.normalized() * delta * 300);
}
