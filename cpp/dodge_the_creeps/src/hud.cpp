#include "hud.hpp"

void HUD::_ready() {
	_score_label = get_node<godot::Label>("ScoreLabel");
	_message_label = get_node<godot::Label>("MessageLabel");
	_start_message_timer = get_node<godot::Timer>("StartMessageTimer");
	_get_ready_message_timer = get_node<godot::Timer>("GetReadyMessageTimer");
	_start_button = get_node<godot::Button>("StartButton");
	_start_button_timer = get_node<godot::Timer>("StartButtonTimer");
}

// There is no `yield` in GDNative, so we need to have every
// step be its own method that is called on timer timeout.
void HUD::show_get_ready() {
	_message_label->set_text("Get Ready");
	_message_label->show();
	_get_ready_message_timer->start();
}

void HUD::show_game_over() {
	_message_label->set_text("Game Over");
	_message_label->show();
	_start_message_timer->start();
}

void HUD::update_score(const int p_score) {
	_score_label->set_text(godot::Variant(p_score));
}

void HUD::_on_StartButton_pressed() {
	_start_button_timer->stop();
	_start_button->hide();
	emit_signal("start_game");
}

void HUD::_on_StartMessageTimer_timeout() {
	_message_label->set_text("Dodge the\nCreeps");
	_message_label->show();
	_start_button_timer->start();
}

void HUD::_on_GetReadyMessageTimer_timeout() {
	_message_label->hide();
}

void HUD::_register_methods() {
	godot::register_method("_ready", &HUD::_ready);
	godot::register_method("show_get_ready", &HUD::show_get_ready);
	godot::register_method("show_game_over", &HUD::show_game_over);
	godot::register_method("update_score", &HUD::update_score);
	godot::register_method("_on_StartButton_pressed", &HUD::_on_StartButton_pressed);
	godot::register_method("_on_StartMessageTimer_timeout", &HUD::_on_StartMessageTimer_timeout);
	godot::register_method("_on_GetReadyMessageTimer_timeout", &HUD::_on_GetReadyMessageTimer_timeout);
	godot::register_signal<HUD>("start_game", godot::Dictionary());
}
