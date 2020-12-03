#include "application.h"

#include "orange/Events/ApplicationEvent.h"
#include "orange/Log.h"


namespace Orange {

	application::application() {}


	application::~application() {}


	void application::run() {
		WindowResizeEvent e(1280, 720);
		OG_TRACE(e);

		while (true);
	}
}