extends Control

# load the SIMPLE library
onready var data = preload("res://SIMPLE.gdn").new()

func _ready():
	# Called every time the node is added to the scene.
	# Initialization here
	pass


func _on_Button_pressed():
	get_node("Label").set_text("Data = " + data.get_data())
