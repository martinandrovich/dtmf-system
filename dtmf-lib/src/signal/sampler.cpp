#include <thread>
#include <chrono>
#include <vector>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>

#include "sampler.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace sampler
{
	// Private Constructs
	class recorder : public sf::SoundRecorder
	{
	public:
		recorder();
		~recorder();

	protected:
		virtual bool onStart() override;
		virtual void onStop() override;
		virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override;
	};
	
	// Private Members
	int					rate;
	int					interval;
	state				status		= state::unitialized;
	std::vector<float>	buffer;
	std::thread			worker;
	recorder*			rec;

	void(*callback)(std::vector<short> data);

	// Private Methods
	void thread();
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize the sampler
void sampler::init(void(*callback)(std::vector<short> data), int interval, int rate)
{
	sampler::interval = interval;
	sampler::rate = rate;
	sampler::callback = callback;

	if (recorder::isAvailable())
		rec = new recorder;

	//sampler::worker = std::thread(&sampler::thread);

	sampler::status = state::idle;
}

// Start the sampler
void sampler::run()
{
	if (sampler::status == state::unitialized)
		return;

	sampler::status = state::running;
}

// End the sampler
void sampler::end()
{
	if (sampler::status == state::unitialized)
		return;

	sampler::status = state::unitialized;
	sampler::worker.join();

	rec->stop();
	delete rec;
}

// Thread function
void sampler::thread()
{
	using namespace std::literals::chrono_literals;

	while (true)
	{
		if (sampler::status != state::running)
			continue;

		// Do some sampling
		std::cout << "[SAMPLING] Generating samples...\n";
		std::vector<short> fakeData = { 1, 2, 3 };

		std::this_thread::sleep_for(5000ms);

		sampler::callback(fakeData);	
	}
}

// Return the status variable
sampler::state sampler::getState()
{
	return sampler::status;
}


//// Constructs Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////

// Custom Recorder Constructor
sampler::recorder::recorder()
{
	std::cout << sampler::interval << std::endl;
	this->setProcessingInterval(sf::milliseconds(sampler::interval));
	this->start();
}

// Custom Recorder Destructor
sampler::recorder::~recorder()
{
	this->stop();
}

bool sampler::recorder::onStart()
{
	return true;
}

void sampler::recorder::onStop() {}

bool sampler::recorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
	const short* data = &samples[0]; // Int16*

	std::vector<short> samplesCopy(data, data + sampleCount);
	sampler::callback(samplesCopy);

	return true;
}