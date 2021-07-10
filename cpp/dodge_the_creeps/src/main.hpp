#ifndef MAIN_H
#define MAIN_H

#include <AudioStreamPlayer.hpp>
#include <CanvasLayer.hpp>
#include <Godot.hpp>
#include <Node.hpp>
#include <PackedScene.hpp>
#include <PathFollow2D.hpp>
#include <RandomNumberGenerator.hpp>
#include <Timer.hpp>

#include "hud.hpp"
#include "player.hpp"

class Main : public godot::Node {
	GODOT_CLASS(Main, godot::Node)

	int score;
	HUD *_hud;
	Player *_player;
	godot::Node2D *_start_position;
	godot::PathFollow2D *_mob_spawn_location;
	godot::Timer *_mob_timer;
	godot::Timer *_score_timer;
	godot::Timer *_start_timer;
	godot::AudioStreamPlayer *_music;
	godot::AudioStreamPlayer *_death_sound;
	godot::Ref<godot::RandomNumberGenerator> _random;

public:
	godot::Ref<godot::PackedScene> mob_scene;

	void _init() {}
	void _ready();
	void game_over();
	void new_game();
	void _on_MobTimer_timeout();
	void _on_ScoreTimer_timeout();
	void _on_StartTimer_timeout();

	static void _register_methods();
};

#endif // MAIN_H
