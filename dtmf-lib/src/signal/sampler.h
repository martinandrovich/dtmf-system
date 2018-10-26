#pragma once
#include "constants.h"

//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

namespace sampler
{
	// Public Members
	enum class state {
		unitialized,
		idle,
		running,
	};
	
	// Public Methods
	void	init(void(*callback)(std::vector<short> data), int interval = SAMPLE_INTERVAL, int rate = SAMPLE_RATE);
	void	run();
	void	end();
	void	flush();

	sampler::state	getState();
}