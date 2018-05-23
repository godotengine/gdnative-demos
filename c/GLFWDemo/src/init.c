#include <gdnative_api_struct.gen.h>
#include <stdio.h>

#include <GLFW/glfw3.h>

struct glfw {
	GLFWwindow *window;
};

const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_ext_nativescript_api_struct *nativescript_api = NULL;

void window_close_callback(GLFWwindow* window)
{
	godot_object *instance = glfwGetWindowUserPointer(window);
	// emit a signal

	printf("user says: \"please close.\"\n");

	{
		// Object.callv("emit_signal", ["closed"])
		static godot_method_bind *mb = NULL;
		if (!mb) {
			mb = api->godot_method_bind_get_method("Object", "callv");
		}

		godot_variant v;
		godot_array args;
		api->godot_array_new(&args);

		godot_string name;
		api->godot_string_new(&name);
		api->godot_string_parse_utf8(&name, "emit_signal");

		{
			godot_string signal_name;
			api->godot_string_new(&signal_name);
			api->godot_string_parse_utf8(&signal_name, "closed");

			godot_variant signal_name_var;
			api->godot_variant_new_string(&signal_name_var, &signal_name);

			api->godot_array_append(&args, &signal_name_var);

			api->godot_variant_destroy(&signal_name_var);
			api->godot_string_destroy(&signal_name);
		}

		const void *c_args[] = {
			&name,
			&args
		};
		api->godot_method_bind_ptrcall(mb, instance, c_args, &v);

		api->godot_string_destroy(&name);
		api->godot_array_destroy(&args);
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


void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options)
{
	api = p_options->api_struct;

	// now find our extensions
	for (int i = 0; i < api->num_extensions; i++) {
		switch (api->extensions[i]->type) {
			case GDNATIVE_EXT_NATIVESCRIPT: {
				nativescript_api = (godot_gdnative_ext_nativescript_api_struct *)api->extensions[i];
			}; break;
			default: break;
		};
	};
}

void GDN_EXPORT godot_nativescript_init(void *p_handle)
{
	if (!glfwInit()) {
		fprintf(stderr, "can't initialize GLFW\n");
		return;
	}

	godot_instance_create_func create = {};
	create.create_func = &glfw_constructor;

	godot_instance_destroy_func destroy = {};
	destroy.destroy_func = &glfw_destructor;

	nativescript_api->godot_nativescript_register_class(p_handle, "GLFW", "Reference", create, destroy);

	{
		godot_instance_method create_window = {};
		create_window.method = &glfw_create_window;

		godot_method_attributes attributes = {};
		attributes.rpc_type = GODOT_METHOD_RPC_MODE_DISABLED;

		nativescript_api->godot_nativescript_register_method(p_handle, "GLFW", "create", attributes, create_window);
	}

	{
		godot_instance_method close_window = {};
		close_window.method = &glfw_close;

		godot_method_attributes attributes = {};
		attributes.rpc_type = GODOT_METHOD_RPC_MODE_DISABLED;

		nativescript_api->godot_nativescript_register_method(p_handle, "GLFW", "close", attributes, close_window);
	}


	{
		godot_instance_method poll_events = {};
		poll_events.method = &glfw_poll_events;

		godot_method_attributes attributes = {};
		attributes.rpc_type = GODOT_METHOD_RPC_MODE_DISABLED;

		nativescript_api->godot_nativescript_register_method(p_handle, "GLFW", "poll_events", attributes, poll_events);
	}

	{
		godot_instance_method is_closed = {};
		is_closed.method = &glfw_is_closed;

		godot_method_attributes attributes = {};
		attributes.rpc_type = GODOT_METHOD_RPC_MODE_DISABLED;

		nativescript_api->godot_nativescript_register_method(p_handle, "GLFW", "is_closed", attributes, is_closed);
	}

	{
		godot_signal signal = {};
		signal.num_args = 0;
		signal.num_default_args = 0;
		api->godot_string_new(&signal.name);
		api->godot_string_parse_utf8(&signal.name, "closed");

		nativescript_api->godot_nativescript_register_signal(p_handle, "GLFW", &signal);

		api->godot_string_destroy(&signal.name);
	}
}

void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *options)
{
	glfwTerminate();
}

godot_variant glfw_create_window(godot_object *instance, void *method_data,
                                 struct glfw *user_data,
                                 int num_args, godot_variant **args)
{
	godot_variant ret;
	api->godot_variant_new_nil(&ret);

	int width = api->godot_variant_as_int(args[0]);
	int height = api->godot_variant_as_int(args[1]);
	godot_string title = api->godot_variant_as_string(args[2]);

	printf("create window: %dx%d\n", width, height);

	{
		godot_char_string title_char_string = api->godot_string_utf8(&title);
		const char *c_title = api->godot_char_string_get_data(&title_char_string);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		user_data->window = glfwCreateWindow(width, height, c_title, NULL, NULL);

		api->godot_char_string_destroy(&title_char_string);

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
	api->godot_variant_new_nil(&ret);

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
	api->godot_variant_new_bool(&ret, user_data->window == NULL);

	return ret;
}


godot_variant glfw_poll_events(godot_object *instance, void *method_data,
                                 struct glfw *user_data,
                                 int num_args, godot_variant **args)
{
	godot_variant ret;
	api->godot_variant_new_nil(&ret);

	glfwPollEvents();

	return ret;
}





void *glfw_constructor(godot_object *instance, void *method_data)
{
	printf("GLFW._init()\n");

	struct glfw *user_data = api->godot_alloc(sizeof(struct glfw));
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

	api->godot_free(user_data);
}
