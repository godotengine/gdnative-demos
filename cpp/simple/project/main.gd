extends Node

const Simple = preload("res://gdnative/simple.gdns")

onready var simple_instance = Simple.new()


func _on_Button_pressed():
	$Label.text = "C++ code says: " + simple_instance.get_data()
