#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>

#include "sampler.h"

// ################################################################################################################################

// ## Private Definitions & Prototyping ###########################################################################################
namespace sampler {

	// Methods
	void perform();
	
	// Variables
	namespace
	{
		enum class state {
			unitialized,
			idle,
			running,
		};

		int				rate		= 44100;
		int				interval	= 50;
		std::thread*	performer;
		state			status		= state::unitialized;

		void(*callback)()			= nullptr;
	}
}

// ## Declarations ################################################################################################################
namespace sampler
{
	void init(void(*callback)())
	{
		std::cout << "Setting up thread...\n";

		sampler::callback = callback;
		performer = new std::thread(&sampler::perform);

		status = state::idle;
	}

	void run()
	{
		if (status == state::unitialized)
			return;

		status = state::running;
	}

	void end()
	{

		status = state::unitialized;

		if (performer != nullptr)
		{
			performer->join();
			delete performer;
		}
	}

	void perform()
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
}