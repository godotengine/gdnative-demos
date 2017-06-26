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

#include <player.h>

#include <AnimatedSprite.hpp>
#include <Input.hpp>
#include <cmath>

using namespace godot;

const static float _gravity = 500.0;

const static float _floor_angle_tolerance = 40.0;
const static float _walk_force = 600.0;
const static float _walk_min_speed = 10.0;
const static float _walk_max_speed = 200.0;
const static float _stop_force = 1300.0;
const static float _jump_speed = 200.0;
const static float _max_airborn_time = 0.2;

const static float _slide_stop_velocity = 1.0;
const static float _slide_stop_min_travel = 1.0;

template <typename T>
int sign(const T &val) {
	return (val > 0) - (val < 0);
}

GDPlayer::GDPlayer() {
}

GDPlayer::~GDPlayer() {
}

void GDPlayer::_init() {
	Vector2 _force = Vector2();

	_jumping = false;
	_on_air_time = 0.0;
	_current_anim = "Default";
}

void GDPlayer::_ready() {
	ray0 = ((RayCast2D *)owner->get_node("Ray0"));
	ray1 = ((RayCast2D *)owner->get_node("Ray1"));

	ray0->add_exception(owner);
	ray1->add_exception(owner);
}

void GDPlayer::_fixed_process(const float delta) {
	Vector2 _force = Vector2(0, _gravity);

	bool left = Input::is_action_pressed("ui_left");
	bool right = Input::is_action_pressed("ui_right");
	bool jump = Input::is_action_pressed("ui_up");

	bool stop = true;

	if (left) {
		if (_velocity.x <= _walk_min_speed && _velocity.x > -_walk_max_speed) {
			_force.x -= _walk_force;
			stop = false;
		}
	} else if (right) {
		if (_velocity.x >= -_walk_min_speed && _velocity.x < _walk_max_speed) {
			_force.x += _walk_force;
			stop = false;
		}
	}

	if (stop) {
		float vsign = sign(_velocity.x);
		float vlen = std::abs(_velocity.x);

		vlen -= _stop_force * delta;

		if (vlen < 0) {
			vlen = 0;
		}

		_velocity.x = (vlen * vsign);
	}

	// Integrate forces to velocity
	_velocity += _force * delta;
	// Integrate velocity into motion and move
	_velocity = owner->move_and_slide(_velocity, Vector2(0, -1));

	bool floor_colliding = (ray0->is_colliding() || ray1->is_colliding());

	if (owner->is_on_floor()) {
		_on_air_time = 0;
	}

	if (_on_air_time < _max_airborn_time && jump && !_prev_jump_pressed && !_jumping) {
		_velocity.y = -_jump_speed;
		_jumping = false;
	}

	_on_air_time += delta;
	_prev_jump_pressed = jump;

	/**
	 * @brief Animating
	 *
	 * The following show's how to get the child nodes and access there functions..
	 */

	bool animating = false;

	if (left) {
		((AnimatedSprite *)owner->get_node("AnimatedSprite"))->set_flip_h(true);
		if (floor_colliding) {
			_current_anim = "Run";
		}
		animating = true;
	}
	if (right) {
		((AnimatedSprite *)owner->get_node("AnimatedSprite"))->set_flip_h(false);
		if (floor_colliding) {
			_current_anim = "Run";
		}
		animating = true;
	}

	if (!floor_colliding) {
		if (_velocity.y > 0) {
			_current_anim = "JumpDown";
		}
		if (_velocity.y < 0) {
			_current_anim = "JumpUp";
		}
	} else if (!animating) {
		_current_anim = "Default";
	}

	((AnimatedSprite *)owner->get_node("AnimatedSprite"))->play(_current_anim);
}

void GDPlayer::_register_methods() {

	register_method((char *)"_init", &GDPlayer::_init);
	register_method((char *)"_ready", &GDPlayer::_ready);

	register_method((char *)"_fixed_process", &GDPlayer::_fixed_process);

	register_signal<GDPlayer>("move");
}
