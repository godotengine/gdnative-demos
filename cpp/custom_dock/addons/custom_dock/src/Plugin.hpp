#ifndef PLUGIN_H
#define PLUGIN_H

#include <Control.hpp>
#include <EditorPlugin.hpp>
#include <Godot.hpp>
#include <PackedScene.hpp>
#include <Reference.hpp>

using namespace godot;

class Plugin : public EditorPlugin {
	GODOT_CLASS(Plugin, EditorPlugin)

public:
	Plugin();
	~Plugin();

	void _init();
	void _ready();
	void _enter_tree();
	void _exit_tree();
	static void _register_methods();

private:
	Ref<PackedScene> plugin_scene;
	Control *plugin_instance;
};

#endif
