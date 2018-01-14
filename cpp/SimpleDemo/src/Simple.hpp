#include <Godot.hpp>

#include <Reference.hpp>

class Simple : public godot::GodotScript<godot::Reference> {
	GODOT_CLASS(Simple)
	
	godot::String data;
public:

	static void _register_methods();

	void _init();

	godot::String get_data() const;
};
