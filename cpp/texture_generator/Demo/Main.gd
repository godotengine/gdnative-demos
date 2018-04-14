extends Spatial

var x_angle = 0.0
var y_angle = 0.0
var half_pi = PI * 0.5
var mouse_sensitivity = 0.01

func _input(event):
	if event is InputEventMouseMotion and Input.is_mouse_button_pressed(1):
		x_angle -= event.relative.x * mouse_sensitivity
		y_angle -= event.relative.y * mouse_sensitivity
		
		# limit our up/down angle
		if y_angle > half_pi:
			y_angle = half_pi
		elif y_angle < -half_pi:
			y_angle = -half_pi

		var new_basis = Basis()
		new_basis = new_basis.rotated(Vector3(1.0, 0.0, 0.0), y_angle)
		new_basis = new_basis.rotated(Vector3(0.0, 1.0, 0.0), x_angle)
		$Test.transform.basis = new_basis

func _on_Quit_pressed():
	get_tree().quit()
