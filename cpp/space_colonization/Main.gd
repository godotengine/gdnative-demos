extends Spatial

# class member variables go here, for example:
# var a = 2
# var b = "textvar"

onready var treemesh = preload("res://mesh_tree.gdns")

func _ready():
	$Tree.mesh = treemesh.new()
	

	set_process(true)
	
func _process(delta):
	$Tree.rotate_y(delta)