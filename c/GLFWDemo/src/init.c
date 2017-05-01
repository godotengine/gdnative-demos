#include <godot.h>
#include <stdio.h>

#include <GLFW/glfw3.h>

struct glfw {
	GLFWwindow *window;
};




void window_close_callback(GLFWwindow* window)
{
	godot_object *instance = glfwGetWindowUserPointer(window);
	// emit a signal

	printf("user says: \"please close.\"\n");

	{
		// Object.callv("emit_signal", ["closed"])
		static godot_method_bind *mb = NULL;
		if (!mb) {
			mb = godot_method_bind_get_method("Object", "callv");
		}

		godot_variant v;
		godot_array args;
		godot_array_new(&args);

		godot_string name;
		godot_string_new_data(&name, "emit_signal", 11);

		godot_string_destroy(&name);

		{
			godot_string signal_name;
			godot_string_new_data(&signal_name, "closed", 6);

			godot_variant signal_name_var;
			godot_variant_new_string(&signal_name_var, &signal_name);

			godot_array_append(&args, &signal_name_var);

			godot_variant_destroy(&signal_name_var);
			godot_string_destroy(&signal_name);
		}

		const void *c_args[] = {
			&name,
			&args
		};
		godot_method_bind_ptrcall(mb, instance, c_args, &v);

		godot_string_destroy(&name);
		godot_array_destroy(&args);
	}
}







void *glfw_constructor(godot_object *instance, void *method_data);
void glfw_destructor(godot_object *instance, void *method_data, struct glfw *user_data);

godot_variant glfw_create_window(godot_object *instance, void *method_data,
                                 struct glfw *user_data,
                                 int num_args, godot_variant **args);

godot_variant glfw_close(godot_object *instance, void *method_data,
                                 struct glfw *user_data,
                                 int num_args, godot_variant **args);

godot_variant glfw_poll_events(godot_object *instance, void *method_data,
                                 struct glfw *user_data,
                                 int num_args, godot_variant **args);


godot_variant glfw_is_closed(godot_object *instance, void *method_data,
                                 struct glfw *user_data,
                                 int num_args, godot_variant **args);


void godot_native_init(godot_native_init_options *options)
{
	if (!glfwInit()) {
		fprintf(stderr, "can't initialize GLFW\n");
		return;
	}

	godot_instance_create_func create = {};
	create.create_func = &glfw_constructor;

	godot_instance_destroy_func destroy = {};
	destroy.destroy_func = &glfw_destructor;

	godot_script_register_class("GLFW", "Reference", create, destroy);

	{
		godot_instance_method create_window = {};
		create_window.method = &glfw_create_window;

		godot_method_attributes attributes = {};
		attributes.rpc_type = GODOT_METHOD_RPC_MODE_DISABLED;

		godot_script_register_method("GLFW", "create", attributes, create_window);
	}

	{
		godot_instance_method close_window = {};
		close_window.method = &glfw_close;

		godot_method_attributes attributes = {};
		attributes.rpc_type = GODOT_METHOD_RPC_MODE_DISABLED;

		godot_script_register_method("GLFW", "close", attributes, close_window);
	}


	{
		godot_instance_method poll_events = {};
		poll_events.method = &glfw_poll_events;

		godot_method_attributes attributes = {};
		attributes.rpc_type = GODOT_METHOD_RPC_MODE_DISABLED;

		godot_script_register_method("GLFW", "poll_events", attributes, poll_events);
	}

	{
		godot_instance_method is_closed = {};
		is_closed.method = &glfw_is_closed;

		godot_method_attributes attributes = {};
		attributes.rpc_type = GODOT_METHOD_RPC_MODE_DISABLED;

		godot_script_register_method("GLFW", "is_closed", attributes, is_closed);
	}

	{
		godot_signal signal = {};
		signal.num_args = 0;
		signal.num_default_args = 0;
		godot_string_new_data(&signal.name, "closed", 6);

		godot_script_register_signal("GLFW", &signal);

		godot_string_destroy(&signal.name);
	}
}

void godot_native_terminate(godot_native_terminate_options *options)
{
	glfwTerminate();
}




godot_variant glfw_create_window(godot_object *instance, void *method_data,
                                 struct glfw *user_data,
                                 int num_args, godot_variant **args)
{
	godot_variant ret;
	godot_variant_new_nil(&ret);

	int width = godot_variant_as_int(args[0]);
	int height = godot_variant_as_int(args[1]);;
	godot_string title = godot_variant_as_string(args[2]);

	printf("create window: %dx%d\n", width, height);

	{
		int length;
		godot_string_get_data(&title, NULL, &length);
		
		char c_title[length + 1];
		c_title[length] = 0;
		godot_string_get_data(&title, c_title, &length);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	
		user_data->window = glfwCreateWindow(width, height, c_title, NULL, NULL);

		glfwSetWindowUserPointer(user_data->window, instance);

		glfwSetWindowCloseCallback(user_data->window, window_close_callback);
	}

	
	

	return ret;
}



godot_variant glfw_close(godot_object *instance, void *method_data,
                                 struct glfw *user_data,
                                 int num_args, godot_variant **args)
{
	godot_variant ret;
	godot_variant_new_nil(&ret);

	if (user_data->window) {
		glfwDestroyWindow(user_data->window);
		user_data->window = NULL;
	}

	return ret;
}


godot_variant glfw_is_closed(godot_object *instance, void *method_data,
                                 struct glfw *user_data,
                                 int num_args, godot_variant **args)
{
	godot_variant ret;
	godot_variant_new_bool(&ret, user_data->window == NULL);

	return ret;
}


godot_variant glfw_poll_events(godot_object *instance, void *method_data,
                                 struct glfw *user_data,
                                 int num_args, godot_variant **args)
{
	godot_variant ret;
	godot_variant_new_nil(&ret);

	glfwPollEvents();

	return ret;
}





void *glfw_constructor(godot_object *instance, void *method_data)
{
	printf("GLFW._init()\n");
	
	struct glfw *user_data = godot_alloc(sizeof(struct glfw));
	user_data->window = NULL;

	return user_data;
}

void glfw_destructor(godot_object *instance, void *method_data, struct glfw *user_data)
{
	// destroy window
	
	if (user_data->window) {
		glfwDestroyWindow(user_data->window);
		user_data->window = NULL;
	}

	godot_free(user_data);
}
