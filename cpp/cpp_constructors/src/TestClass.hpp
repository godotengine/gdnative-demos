#include <Reference.hpp>
#include <Timer.hpp>
#include <File.hpp>

#include <Ref.hpp>


#include <stdio.h>

using namespace godot;

class TestClass : public Reference {
	GODOT_CLASS(TestClass, Reference)

	int count;

public:

	void _init() {
		count = 34;
	}

	int test_method(String s) {
		Godot::print(s);
		count++;

		Timer *t = new Timer;

		t->set_wait_time(13.36 * count);

		Godot::print(String::num_real(t->get_wait_time()));

		delete t;

		// test Ref<T>
		Ref<File> file;
		file.instance();

		file->open("res://test.txt", File::WRITE);

		file->close();

		// ref should free the memory automatically

		return count;
	}

	static void _register_methods() {
		register_method("test_method", &TestClass::test_method);
	}
};
