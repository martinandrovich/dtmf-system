#pragma once
#include <bitset>

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
	void init(void(*callback)(std::bitset<3> payload));
	void run();
	void end();
}