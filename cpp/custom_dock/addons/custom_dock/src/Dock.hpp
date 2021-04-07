#ifndef DOCK_H
#define DOCK_H

#include <Color.hpp>
#include <Godot.hpp>
#include <Panel.hpp>

using namespace godot;

class Dock : public Panel {
	GODOT_CLASS(Dock, Panel)

public:
	Dock();
	~Dock();

	void _init();
	void _on_HelloButton_pressed();
	void _on_PrintButton_pressed();
	void _on_HSlider_value_changed(float);
	void _on_ColorPickerButton_color_changed(Color);
	static void _register_methods();
};

#endif
