#include "mob.hpp"

#include <RandomNumberGenerator.hpp>
#include <SpriteFrames.hpp>

void Mob::_ready() {
	godot::Ref<godot::RandomNumberGenerator> random = godot::RandomNumberGenerator::_new();
	random->randomize();
	_animated_sprite = get_node<godot::AnimatedSprite>("AnimatedSprite");
	_animated_sprite->_set_playing(true);
	godot::PoolStringArray mob_types = _animated_sprite->get_sprite_frames()->get_animation_names();
	_animated_sprite->set_animation(mob_types[random->randi() % mob_types.size()]);
}

void Mob::_on_VisibilityNotifier2D_screen_exited() {
	queue_free();
}

void Mob::_on_start_game() {
	queue_free();
}

void Mob::_register_methods() {
	godot::register_method("_ready", &Mob::_ready);
	godot::register_method("_on_VisibilityNotifier2D_screen_exited", &Mob::_on_VisibilityNotifier2D_screen_exited);
	godot::register_method("_on_start_game", &Mob::_on_start_game);
}
