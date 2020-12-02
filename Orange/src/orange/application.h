#pragma once

#include "core.h"


namespace Orange {

	class ORANGE_API application {

	public:
		application();
		virtual ~application();

		void run();
	};

	// To be defined in CLIENT
	application* create_application();
}
