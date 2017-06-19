extends Control

onready var TestClass = preload("res://TestClass.gdn")

func _ready():
	var test_object = TestClass.new()
	
	test_object.test_method("Hello World")
