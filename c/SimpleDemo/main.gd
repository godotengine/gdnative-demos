extends Control

# load the SIMPLE library
onready var data = preload("res://bin/simple.gdns").new()

func _on_Button_pressed():
	$Label.text = "Data = " + data.get_data()
