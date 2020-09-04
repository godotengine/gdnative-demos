extends Node

# Load the GLFW library.
onready var window = preload("res://gdnative/glfw.gdns").new()

func _ready():
	# Create a 400x400 px window.
	window.create(400, 400, "Hello from Godot!")

	# Connect the "closed" signal.
	window.connect("closed", self, "on_window_close")


func on_window_close():
	# The window emits a signal when the users requests
	# to close it. So let's close it :D
	window.close()


func _process(_delta):
	# If the window is still open, poll for events
	# on every frame.
	if not window.is_closed():
		window.poll_events()


func _notification(what):
	match what:
		NOTIFICATION_PREDELETE:
			# If the game closes, the window should close
			# as well. `window.close()` works when
			# window already is closed.
			window.close()
