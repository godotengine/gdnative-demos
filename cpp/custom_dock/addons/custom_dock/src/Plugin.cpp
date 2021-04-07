#include <Godot.hpp>
#include <ResourceLoader.hpp>

#include "Plugin.hpp"
using namespace godot;

Plugin::Plugin() {
}

Plugin::~Plugin() {
	this->plugin_instance->free();
}

void Plugin::_init() {
}

void Plugin::_ready() {
}

void Plugin::_enter_tree() {
	this->plugin_scene = ResourceLoader::get_singleton()->load("res://addons/custom_dock/Scenes/MainPanel.tscn");
	this->plugin_instance = cast_to<Control>(this->plugin_scene->instance());
	EditorPlugin::add_control_to_dock(0, this->plugin_instance);
}

void Plugin::_exit_tree() {
	EditorPlugin::remove_control_from_docks(this->plugin_instance);
}

void Plugin::_register_methods() {
	register_method((char *)"_init", &Plugin::_init);
	register_method((char *)"_ready", &Plugin::_ready);
	register_method((char *)"_enter_tree", &Plugin::_enter_tree);
	register_method((char *)"_exit_tree", &Plugin::_exit_tree);
}
