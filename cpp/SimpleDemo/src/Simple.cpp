#include "Simple.hpp"

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
