#include "Simple.hpp"

#include "Input.hpp"

void Simple::_register_methods()
{
	godot::register_method("get_data", &Simple::get_data);
}

void Simple::_init()
{
	data = "Hello World from C++";
}

godot::String Simple::get_data() const
{
	return data;
}

void SimpleSprite::_register_methods()
{
	godot::register_method("_process", &SimpleSprite::_process);
}

void SimpleSprite::_init()
{
	godot::Godot::print("Wheeeeey");
}

void SimpleSprite::_process(double delta)
{
	godot::Vector2 input_dir(0, 0);

	if (godot::Input::get_singleton()->is_action_pressed("ui_right")) {
		input_dir.x += 1;
	}
	if (godot::Input::get_singleton()->is_action_pressed("ui_left")) {
		input_dir.x -= 1;
	}
	if (godot::Input::get_singleton()->is_action_pressed("ui_down")) {
		input_dir.y += 1;
	}
	if (godot::Input::get_singleton()->is_action_pressed("ui_up")) {
		input_dir.y -= 1;
	}

	set_position(get_position() + input_dir.normalized() * delta * 300);
}
