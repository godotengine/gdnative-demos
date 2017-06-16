#include <godot.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct user_data_struct {
	char data[256];
} user_data_struct;

void *simple_constructor(godot_object *instance, void *method_data);
void simple_destructor(godot_object *instance, void *method_data, user_data_struct *user_data);
godot_variant simple_get_data(godot_object *instance, void *method_data, user_data_struct *user_data, int num_args, godot_variant **args);

void godot_native_init(godot_native_init_options *options) {

	godot_instance_create_func create = {};
	create.create_func = &simple_constructor;

	godot_instance_destroy_func destroy = {};
	destroy.destroy_func = &simple_destructor;

	godot_script_register_class("SIMPLE", "Reference", create, destroy);

	{
		godot_instance_method get_data = {};
		get_data.method = &simple_get_data;

		godot_method_attributes attributes = {};
		attributes.rpc_type = GODOT_METHOD_RPC_MODE_DISABLED;

		godot_script_register_method("SIMPLE", "get_data", attributes, get_data);
	}

}

void *simple_constructor(godot_object *instance, void *method_data) {
	printf("SIMPLE._init()\n");
	
	user_data_struct *user_data = godot_alloc(sizeof(user_data_struct));
	strcpy(user_data->data, "World");

	return user_data;
}

void simple_destructor(godot_object *instance, void *method_data, user_data_struct *user_data) {
	printf("SIMPLE._byebye()\n");

	godot_free(user_data);
}

godot_variant simple_get_data(godot_object *instance, void *method_data, user_data_struct *user_data, int num_args, godot_variant **args) {
	godot_string data;
	godot_variant ret;

	godot_string_new_data(&data, user_data->data, strlen(user_data->data));
	godot_variant_new_string(&ret, &data);

	return ret;
}
