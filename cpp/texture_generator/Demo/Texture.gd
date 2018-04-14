extends MeshInstance

onready var gen_tex = preload("res://addons/gdtexturegen/gen_texture.gdns").new()

func _ready():
	# Size our texture, note for 2D textures just leave z on 1
	gen_tex.set_texture_size(128, 128, 128)
	
	# If you want to use the worley texture generator you must first create a point cloud.
	# The point cloud gets rebuild if this is called again
	gen_tex.generate_worley_point_cloud(10)
	
	# Load up our worley textures, note our octaves should be whole numbers
	gen_tex.simplex_texture(1.0, 1.0, 0, false)
#	gen_tex.worley_texture(1.0, 1.0, 0, false)
	gen_tex.worley_texture(1,0, 2.0, 1, false)
	gen_tex.worley_texture(1.0, 3.0, 2, false)
	gen_tex.worley_texture(1.0, 4.0, 3, false)
	
	# and finalize, this will load what we've generated into a texture Godot understands
	gen_tex.bake_texture()
	
	# apply our material to our cube
	var material = get_surface_material(0)
	material.set_shader_param("albedo_texture", gen_tex)
	
	# strangely enough, if we don't reset our transform there is a weird redraw issue
	transform.basis = Basis()
