#pragma once
#include <vector>
#include <iostream>

#include "sampler.h"

//// Constructor & Destructor /////////////////////////////////////////////////////////////////////////////////////////////////////

// Default Constructor
sampler::sampler(void(*callback)(std::vector<short> samples))
	: callback(callback), rate(SAMPLE_RATE), interval(SAMPLE_INTERVAL), status(state::idle)
{
	// set processing interval
	this->setProcessingInterval(sf::milliseconds(this->interval));

	// log
	std::cout << "\nInitialized sampler with:\n";
	std::cout << "SAMPLE RATE:\t\t"			<< this->rate << " Hz \n";
	std::cout << "SAMPLE INTERVAL:\t"		<< this->interval << " ms \n";	
}

// Destructor
sampler::~sampler()
{
	this->stop();
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Return sampler status
sampler::state sampler::getStatus()
{
	return this->status;
}

// EVENT: Start the sampler
bool sampler::onStart()
{
	this->status = state::sampling;
	return true;
}

// EVENT: Stop the sampler
void sampler::onStop()
{
	this->status = state::idle;
}

// EVENT: Process a chunk of samples
bool sampler::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{	
	if (this->status == state::idle)
		return true;
	
	this->status = state::processing;

	const short* data = &samples[0]; // Int16*
	std::vector<short> samplesCopy(data, data + sampleCount);

	// callback with copy of samples chunk
	this->callback(samplesCopy);

	// continue sampling
	this->status = state::sampling;
	return true;
}