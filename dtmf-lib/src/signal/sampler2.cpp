#include "sampler2.h"

//// Global Variables /////////////////////////////////////////////////////////////////////////////////////////////////////////////
int					sampler2::rate = 44100;
int					sampler2::interval = 50;
sampler2::state		sampler2::status = sampler2::state::unitialized;
std::vector<int>	sampler2::buffer;
std::thread			sampler2::performer;

void(*sampler2::callback)() = nullptr;


//// Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void sampler2::init(void(*callback)())
{
	std::cout << "Setting up thread...\n";

	sampler2::callback = callback;
	performer = std::thread(&sampler2::perform);

	status = state::idle;
}

void sampler2::run()
{
	if (status == state::unitialized)
		return;

	status = state::running;
}

void sampler2::end()
{
	if (status == state::unitialized)
		return;
	
	status = state::unitialized;
	performer.join();
	
}

void sampler2::perform()
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