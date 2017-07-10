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

#include <colworld.h>
#include <Label.hpp>

using namespace godot;

ColWorld::ColWorld() {
}

ColWorld::~ColWorld() {
}

void ColWorld::_init() {
}

void ColWorld::_ready() {
	owner->get_node("princess")->connect("body_entered", owner, "_on_princess_body_enter");
}

void ColWorld::_on_princess_body_enter(KinematicBody2D *body) {
	if (body->get_name() == "Character") {
		((Label *)owner->get_node("youwin"))->show();
	}
}

void ColWorld::_register_methods() {
	register_method((char *)"_init", &ColWorld::_init);
	register_method((char *)"_ready", &ColWorld::_ready);

	register_method((char *)"_on_princess_body_enter", &ColWorld::_on_princess_body_enter);
}
