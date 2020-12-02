#include <orange.h>


class Sandbox : public Orange::application {

public:
	Sandbox() {

	}


	~Sandbox() {

	}
};


Orange::application* Orange::create_application() {
	return new Sandbox();
}