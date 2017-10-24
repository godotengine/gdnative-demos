/**
 * Copyright 2017 RameshRavone. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#ifndef PLAYER_H
#define PLAYER_H

#include <Godot.hpp>

#include <InputEvent.hpp>
#include <RayCast2D.hpp>

#include <KinematicBody2D.hpp>

using namespace godot;

class GDPlayer : public GodotScript<KinematicBody2D> {
	GODOT_CLASS(GDPlayer)

public:
	GDPlayer();
	~GDPlayer();

	void _init();
	void _ready();
	void moving();

	void _physics_process(const float delta);

	static void _register_methods();

private:
	bool _jumping;
	bool _prev_jump_pressed;

	float _on_air_time;

	Vector2 _velocity;
	String _current_anim;

	RayCast2D *ray0;
	RayCast2D *ray1;
};

#endif // PLAYER_H_
