#pragma once
#include <vector>
#include <iostream>

#include "sampler2.h"

//// Constructor & Destructor /////////////////////////////////////////////////////////////////////////////////////////////////////

// Default Constructor
sampler::sampler(void(*callback)(std::vector<short> samples))
	: callback(callback), rate(SAMPLE_RATE), interval(SAMPLE_INTERVAL), status(state::idle)
{
	// set processing interval
	this->setProcessingInterval(sf::milliseconds(this->interval));

	// log
	std::cout << "Initialized sampler with:\n";
	std::cout << "SAMPLE RATE:\t\t"			<< this->rate << "Hz \n";
	std::cout << "SAMPLE INTERVAL:\t\t "	<< this->interval << "ms \n";	
}

// Destructor
sampler::~sampler()
{
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// ...
void sampler::run()
{
	this->status = state::sampling;
	this->start();
}

// ...
sampler::state sampler::getStatus()
{
	return this->status;
}

// ...
bool sampler::onStart()
{
	return true;
}

// ...
void sampler::onStop()
{
}

// ...
bool sampler::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
	if (this->status == state::idle)
		return true;
	
	this->status = state::processing;
	const short* data = &samples[0]; // Int16*

	std::vector<short> samplesCopy(data, data + sampleCount);
	this->callback(samplesCopy);

	this->status = state::sampling;
	return true;
}