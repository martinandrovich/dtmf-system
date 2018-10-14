#include <thread>
#include <chrono>
#include <vector>
#include <iostream>

#include "sampler.h"

//// Private Definitions & Prototyping ////////////////////////////////////////////////////////////////////////////////////////////
namespace sampler
{

	// Private Methods
	void perform();
	
	// Private Variables
	namespace
	{
		enum class state {
			unitialized,
			idle,
			running,
		};

		int					rate		= 44100;
		int					interval	= 50;
		state				status		= state::unitialized;
		std::vector<int>	buffer;
		std::thread*		performer;

		void(*callback)()				= nullptr;
	}
}

//// Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void sampler::init(void(*callback)())
{
	std::cout << "Setting up thread...\n";

	sampler::callback = callback;
	performer = new std::thread(&sampler::perform);

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

	status = state::unitialized;

	if (performer != nullptr)
	{
		performer->join();
		delete performer;
	}
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