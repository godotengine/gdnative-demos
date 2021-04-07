#include <gdnative_api_struct.gen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_ext_nativescript_api_struct *nativescript_api = NULL;
const godot_gdnative_ext_nativescript_1_1_api_struct *nativescript_1_1_api = NULL;

GDCALLINGCONV void *ibd_constructor(godot_object *instance, void *method_data);
GDCALLINGCONV void ibd_destructor(godot_object *instance, void *method_data, void *user_data);

//
// instance binding functions
//

int language_binding_index = -1;

typedef struct wrapper_object {
	godot_object *object; // the actual object

	// some test data
	char *message;
} wrapper_object;

void *create_wrapper_object(void *data, const void *global_type_tag, godot_object *object) {
	printf("we are now creating a wrapper object...\n");
	fflush(stdout);

	wrapper_object *wrapper = api->godot_alloc(sizeof(wrapper_object));

	wrapper->object = object;

	wrapper->message = "This is a wrapper object.";

	return wrapper;
}

void destroy_wrapper_object(void *data, void *wrapper) {
	api->godot_free(wrapper);

	printf("the wrapper object got destroyed.\n");
	fflush(stdout);
}

void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *options) {
	api = options->api_struct;

	// now find our extensions
	for (unsigned int i = 0; i < api->num_extensions; i++) {
		switch (api->extensions[i]->type) {
			case GDNATIVE_EXT_NATIVESCRIPT: {
				nativescript_api = (godot_gdnative_ext_nativescript_api_struct *)api->extensions[i];

				if (!nativescript_api->next)
					break;

				if (nativescript_api->next->version.major == 1 && nativescript_api->next->version.minor == 1) {
					nativescript_1_1_api = (const godot_gdnative_ext_nativescript_1_1_api_struct *)nativescript_api->next;
				}
			}; break;
			default:
				break;
		};
	};
}

void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *options) {
	api = NULL;
	nativescript_api = NULL;
	nativescript_1_1_api = NULL;
}

void GDN_EXPORT godot_nativescript_init(void *handle) {
	printf("nativescript_init\n");

	fflush(stdout);

	// register instance binding functions
	{
		godot_instance_binding_functions bind = {0};
		bind.alloc_instance_binding_data = &create_wrapper_object;
		bind.free_instance_binding_data = &destroy_wrapper_object;

		language_binding_index = nativescript_1_1_api->godot_nativescript_register_instance_binding_data_functions(bind);
	}

	// register class
	{
		godot_instance_create_func constructor = {0};
		constructor.create_func = &ibd_constructor;

		godot_instance_destroy_func destructor = {0};
		destructor.destroy_func = &ibd_destructor;

		nativescript_api->godot_nativescript_register_class(handle, "InstanceBinding", "Reference", constructor, destructor);
	}
}

void GDN_EXPORT godot_nativescript_terminate(void *handle) {
	nativescript_1_1_api->godot_nativescript_unregister_instance_binding_data_functions(language_binding_index);
}

GDCALLINGCONV void *ibd_constructor(godot_object *instance, void *method_data) {
	wrapper_object *wrapper = nativescript_1_1_api->godot_nativescript_get_instance_binding_data(language_binding_index, instance);

	printf("message is: %s\n", wrapper->message);
	fflush(stdout);

	return NULL;
}

GDCALLINGCONV void ibd_destructor(godot_object *instance, void *method_data, void *user_data) {
}
