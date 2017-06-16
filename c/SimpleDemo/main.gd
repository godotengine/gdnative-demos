extends Control

# load the SIMPLE library
onready var data = preload("res://SIMPLE.gdn").new()

func _on_Button_pressed():
	$Label.text = "Data = " + data.get_data()
