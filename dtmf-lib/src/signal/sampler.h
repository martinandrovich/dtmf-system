#pragma once

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
	void	init(void(*callback)(std::vector<float> data));
	void	run();
	void	end();
	void	flush();

	sampler::state	getState();
}