#pragma once
#include <vector>
#include <array>
#include <cmath>
#include <iostream>

#include "generator.h"

// Constructor & Destructor
generator::generator() {}
generator::~generator() {}

sf::SoundBuffer* generator::generateSamples(int f1, int f2, int duration, int amplitude, double fade)
{
	auto sampleSize = SAMPLE_RATE * (duration/1000.);
	int fadeValue = fade * sampleSize;
	std::vector<sf::Int16> temp(sampleSize);
	
	auto tempSoundbuffer = new sf::SoundBuffer; // NEEDS GARBAGE CLEANING !!!!!!
	int amplitudeFinal;

	for (unsigned int i = 0; i < sampleSize; i++)
	{
		if (i < fadeValue) {
			amplitudeFinal = (i / (fadeValue * 1.f)) * amplitude;
		}
		else if (i > sampleSize - fadeValue) {
			amplitudeFinal = ((sampleSize - i) / (fadeValue * 1.f)) * amplitude;
		}
		else
		{
			amplitudeFinal = amplitude;
		}

		temp[i] = amplitudeFinal * (sin(f1 * 2 * PI*(i * 1.f / SAMPLE_RATE)) + sin(f2 * 2 * PI*(i * 1.f / SAMPLE_RATE)));

	}

	tempSoundbuffer->loadFromSamples(&temp[0], sampleSize, 1, SAMPLE_RATE);
	return tempSoundbuffer;

}
// Generate an array of samples from a given tone (char 0-D), returned as an SFML SoundBuffer object
sf::SoundBuffer* generator::generateDTMF(int tone, int duration, int amplitude, double fade)
{
	// Opdateres til liste af frekvenser med MOD etc.
	
	int freqLow[4]		= { 697, 770, 852, 941 };
	int freqHigh[4]		= { 1209, 1336, 1477, 1633 };

	return generator::generateSamples(freqLow[tone/4], freqHigh[tone % 4], duration, amplitude);

	switch (tone)
	{
	case 0:
		return generator::generateSamples(941, 1336, duration, amplitude);

	case 1:
		return generator::generateSamples(697, 1209, duration, amplitude);

	default:
		return generator::generateSamples(0, 0, duration, amplitude);
	}
}