#include <thread>
#include <chrono>
#include <vector>
#include <iostream>

#include "sampler.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace sampler
{

	// Private Variables
	int					rate		= 44100;
	int					interval	= 50;
	state				status		= state::unitialized;
	std::vector<int>	buffer;
	std::thread			performer;

	void(*callback)()				= nullptr;

	// Private Methods
	void perform();
	
}

//// Private Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////
void sampler::init(void(*callback)())
{
	std::cout << "Setting up thread...\n";

	sampler::callback = callback;
	performer = std::thread(&sampler::perform);

	status = state::idle;
}

void sampler::run()
{
	if (status == state::unitialized)
		return;

	status = state::running;
}

void sampler::end()
{
	if (status == state::unitialized)
		return;

	status = state::unitialized;
	performer.join();
}

void sampler::perform()
{
	using namespace std::literals::chrono_literals;

	while (true)
	{
		if (status == state::running)
		{
			std::cout << "Thread is running.\n";
			callback();
		}

		std::this_thread::sleep_for(1000ms);
	}
}

sampler::state sampler::getState()
{
	return sampler::status;
}