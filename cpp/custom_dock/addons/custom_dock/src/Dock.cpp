#include <Godot.hpp>
#include <Label.hpp>
#include <LineEdit.hpp>
#include <Panel.hpp>
#include <String.hpp>

#include "Dock.hpp"
using namespace godot;

Dock::Dock() {
}

Dock::~Dock() {
}

void Dock::_init() {
}

void Dock::_on_HelloButton_pressed() {
	Godot::print("Hello World!");
}

void Dock::_on_PrintButton_pressed() {
	String message = ((LineEdit *)get_node(NodePath("VBoxContainer/LineEdit")))->get_text();
	Godot::print(message);
}

void Dock::_on_HSlider_value_changed(float value) {
	((Label *)get_node(NodePath("VBoxContainer/Tabs/Tab/PanelContainer/Label")))->set_text("Slider: " + String::num_int64(value));
}

void Dock::_on_ColorPickerButton_color_changed(Color color) {
	((Label *)get_node(NodePath("VBoxContainer/Tabs/Tab/PanelContainer/Label2")))->set_text("Color: r: " + String::num_int64(color.r * 100) + " g: " + String::num_int64(color.g * 100) + " b: " + String::num_int64(color.b * 100));
}

void Dock::_register_methods() {
	register_method((char *)"_init", &Dock::_init);
	register_method((char *)"_on_HelloButton_pressed", &Dock::_on_HelloButton_pressed);
	register_method((char *)"_on_PrintButton_pressed", &Dock::_on_PrintButton_pressed);
	register_method((char *)"_on_HSlider_value_changed", &Dock::_on_HSlider_value_changed);
	register_method((char *)"_on_ColorPickerButton_color_changed", &Dock::_on_ColorPickerButton_color_changed);
}
