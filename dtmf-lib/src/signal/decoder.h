#pragma once
#include <array>

#include "constants.h"

//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

namespace decoder
{
	// Public Members
	enum class state {
		unitialized,
		running,
		working
	};

	// Public Methods
	void run(std::function<void(uint toneId)>, bool allowPlayback = false);
	void end();
}