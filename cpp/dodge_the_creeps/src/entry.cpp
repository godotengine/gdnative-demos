#include <Godot.hpp>

#include "hud.hpp"
#include "main.hpp"
#include "mob.hpp"
#include "player.hpp"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
	godot::Godot::nativescript_init(handle);

	godot::register_class<HUD>();
	godot::register_class<Main>();
	godot::register_class<Mob>();
	godot::register_class<Player>();
}
