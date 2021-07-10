#include "player.hpp"

#include <Input.hpp>

void Player::_ready() {
	_animated_sprite = get_node<godot::AnimatedSprite>("AnimatedSprite");
	_collision_shape = get_node<godot::CollisionShape2D>("CollisionShape2D");
	_screen_size = get_viewport_rect().size;
	hide();
}

void Player::_process(double p_delta) {
	godot::Input *input = godot::Input::get_singleton();
	godot::Vector2 velocity(0, 0);

	velocity.x = input->get_action_strength("move_right") - input->get_action_strength("move_left");
	velocity.y = input->get_action_strength("move_down") - input->get_action_strength("move_up");

	if (velocity.length() > 0) {
		velocity = velocity.normalized() * speed;
		_animated_sprite->play();
	} else {
		_animated_sprite->stop();
	}

	godot::Vector2 position = get_position();
	position += velocity * p_delta;
	position.x = godot::Math::clamp(position.x, (real_t)0.0, _screen_size.x);
	position.y = godot::Math::clamp(position.y, (real_t)0.0, _screen_size.y);
	set_position(position);

	if (velocity.x != 0) {
		_animated_sprite->set_animation("right");
		_animated_sprite->set_flip_v(false);
		_animated_sprite->set_flip_h(velocity.x < 0);
	} else if (velocity.y != 0) {
		_animated_sprite->set_animation("up");
		_animated_sprite->set_flip_v(velocity.y > 0);
	}
}

void Player::start(godot::Vector2 p_position) {
	set_position(p_position);
	show();
	_collision_shape->set_disabled(false);
}

void Player::_on_Player_body_entered(godot::Node2D *_body) {
	hide(); // Player disappears after being hit.
	emit_signal("hit");
	// Must be deferred as we can't change physics properties on a physics callback.
	_collision_shape->set_deferred("disabled", true);
}

void Player::_register_methods() {
	godot::register_method("_ready", &Player::_ready);
	godot::register_method("_process", &Player::_process);
	godot::register_method("start", &Player::start);
	godot::register_method("_on_Player_body_entered", &Player::_on_Player_body_entered);
	godot::register_property("speed", &Player::speed, 400);
	godot::register_signal<Player>("hit");
}
