#include <Godot.hpp>

#include <Reference.hpp>
#include <Sprite.hpp>

class Simple : public godot::Reference {
	GODOT_CLASS(Simple, godot::Reference)
	
	godot::String data;
public:

	static void _register_methods();

	void _init();

	godot::String get_data() const;
};

class SimpleSprite : public godot::Sprite {
	GODOT_CLASS(SimpleSprite, godot::Sprite)

public:
	static void _register_methods();

	void _init();

	void _process(double delta);
};
