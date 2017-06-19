#include <Godot.hpp>

#include <Reference.hpp>
#include <Timer.hpp>
#include <File.hpp>

#include <Ref.hpp>


#include <stdio.h>

using namespace godot;

class TestClass : public GodotScript<Reference> {
	GODOT_CLASS(TestClass)

	int count;

public:

	int test_method(const String s)
	{
		Godot::print(s);
		count++;

		Timer *t = new Timer;

		t->set_wait_time(13.36 * count);

		// sorry, no String::num() yet
		char str[128];
		snprintf(str, 128, "wait time: %f", t->get_wait_time());

		Godot::print(String(str));

		delete t;

		// test Ref<T>
		Ref<File> file = new File;

		file->open("res://test.txt", File::WRITE);

		file->close();

		// ref should free the memory automatically
		
		
		return count;
	}
	
	static void _register_methods()
	{
		register_method("test_method", &TestClass::test_method);
	}
};

GODOT_NATIVE_INIT(godot_native_init_options *options)
{
	register_class<TestClass>();
}
