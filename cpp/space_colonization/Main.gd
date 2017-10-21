extends Spatial

# class member variables go here, for example:
# var a = 2
# var b = "textvar"

onready var treemesh = preload("res://bin/mesh_tree.gdns")

func _ready():
	var mesh = treemesh.new()
	mesh.generate_attraction_points(1000, 2.0, 1.0, 1.5, 4.0)
	mesh.build_tree(5.0, 0.1, 0.2)
	$Tree.mesh = mesh
	
	set_process(true)
	
func _process(delta):
	$Tree.rotate_y(delta)