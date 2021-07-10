#include "main.hpp"

#include <SceneTree.hpp>

#include "mob.hpp"

void Main::_ready() {
	_hud = get_node<HUD>("HUD");
	_player = get_node<Player>("Player");
	_start_position = get_node<godot::Node2D>("StartPosition");
	_mob_spawn_location = get_node<godot::PathFollow2D>("MobPath/MobSpawnLocation");
	_mob_timer = get_node<godot::Timer>("MobTimer");
	_score_timer = get_node<godot::Timer>("ScoreTimer");
	_start_timer = get_node<godot::Timer>("StartTimer");
	_music = get_node<godot::AudioStreamPlayer>("Music");
	_death_sound = get_node<godot::AudioStreamPlayer>("DeathSound");
	_random = (godot::Ref<godot::RandomNumberGenerator>)godot::RandomNumberGenerator::_new();
	_random->randomize();
}

void Main::game_over() {
	_score_timer->stop();
	_mob_timer->stop();
	_hud->show_game_over();
	_music->stop();
	_death_sound->play();
}

void Main::new_game() {
	get_tree()->call_group("mobs", "queue_free");
	score = 0;
	_player->start(_start_position->get_position());
	_start_timer->start();
	_hud->update_score(score);
	_hud->show_get_ready();
	_music->play();
}

void Main::_on_MobTimer_timeout() {
	// Choose a random location on Path2D.
	_mob_spawn_location->set_offset(_random->randi());

	// Create a Mob instance and add it to the scene.
	godot::Node *mob = mob_scene->instance();
	add_child(mob);

	// Set the mob's direction perpendicular to the path direction.
	real_t direction = _mob_spawn_location->get_rotation() + Math_PI / 2;

	// Set the mob's position to a random location.
	mob->set("position", _mob_spawn_location->get_position());

	// Add some randomness to the direction.
	direction += _random->randf_range(-Math_PI / 4, Math_PI / 4);
	mob->set("rotation", direction);

	// Choose the velocity for the mob.
	godot::Vector2 velocity = godot::Vector2(_random->randf_range(150.0, 250.0), 0.0);
	mob->set("linear_velocity", velocity.rotated(direction));
}

void Main::_on_ScoreTimer_timeout() {
	score += 1;
	_hud->update_score(score);
}

void Main::_on_StartTimer_timeout() {
	_mob_timer->start();
	_score_timer->start();
}

void Main::_register_methods() {
	godot::register_method("_ready", &Main::_ready);
	godot::register_method("game_over", &Main::game_over);
	godot::register_method("new_game", &Main::new_game);
	godot::register_method("_on_MobTimer_timeout", &Main::_on_MobTimer_timeout);
	godot::register_method("_on_ScoreTimer_timeout", &Main::_on_ScoreTimer_timeout);
	godot::register_method("_on_StartTimer_timeout", &Main::_on_StartTimer_timeout);
	godot::register_property("mob_scene", &Main::mob_scene, (godot::Ref<godot::PackedScene>)nullptr);
}
