#pragma once
#include <array>

#include "constants.h"

//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

namespace decoder
{
	// Public Members
	enum class state {
		unitialized,
		idle,
		running,
		working
	};

	// Public Methods
	void init(void(*callback)(uint toneID));
	void run();
	void end();

	int convertDTMF(std::array<int, 2> positions);
}