#ifndef MOB_H
#define MOB_H

#include <AnimatedSprite.hpp>
#include <Godot.hpp>
#include <RigidBody2D.hpp>

class Mob : public godot::RigidBody2D {
	GODOT_CLASS(Mob, godot::RigidBody2D)

	godot::AnimatedSprite *_animated_sprite;

public:
	int min_speed = 150;
	int max_speed = 250;

	void _init() {}
	void _ready();
	void _on_VisibilityNotifier2D_screen_exited();
	void _on_start_game();

	static void _register_methods();
};

#endif // MOB_H
