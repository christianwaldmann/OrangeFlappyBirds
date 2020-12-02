#pragma once


#ifdef OG_PLATFORM_WINDOWS

extern Orange::application* Orange::create_application();

int main(int argc, char** argv) {
	auto app = Orange::create_application();
	app->run();
	delete app;
}

#endif