extends Control

# load the SIMPLE library
const SIMPLE = preload("res://bin/simple.gdns")
onready var data = SIMPLE.new()

func _on_Button_pressed():
	$Label.text = "Data = " + data.get_data()
	
	print(data.get_class())
	print(SIMPLE.class_name)
	
	print("self: ", self.get_class())
	
	var this = get_script()
	
	print(self is this)
	print(data is data.get_script())
