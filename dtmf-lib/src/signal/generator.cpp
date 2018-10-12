#pragma once
#include <vector>
#include <cmath>

#include "generator.h"

// Constructor & Destructor
generator::generator() {}
generator::~generator() {}

// Generate an array of samples from 2 given frequencies, returned as an SFML SoundBuffer object
sf::SoundBuffer* generator::generateSamples(int f1, int f2, int fs, int amplitude, double fade)
{
	sf::Int16 temp[SAMPLES];
	auto tempSoundbuffer = new sf::SoundBuffer; // NEEDS GARBAGE CLEANING !!!!!!
	int ampFinal;

	for (unsigned i = 0; i < SAMPLES - 1; i++)
	{
		if (i < fade) {
			ampFinal = (i / fade) * amplitude;
		}
		else if (i > SAMPLES - fade) {
			ampFinal = ((SAMPLES - i) / (fade)) * amplitude;
		}
		else
		{
			ampFinal = amplitude;
		}

		temp[i] = ampFinal * (sin(f1 * 2 * PI*(i * 1. / fs)) + sin(f2 * 2 * PI*(i * 1. / fs)));

	}
	
	tempSoundbuffer->loadFromSamples(temp, SAMPLES, 1, fs);
	return tempSoundbuffer;
}

// Generate an array of samples from a given tone (char 0-D), returned as an SFML SoundBuffer object
sf::SoundBuffer* generator::generateDTMF(char tone, int amplitude, double fade)
{
	switch (tone)
	{
	case '0':
		return generator::generateSamples(941, 1336, 44100, amplitude, fade);

	case '1':
		return generator::generateSamples(697, 1209, 44100, amplitude, fade);

	default:
		return generator::generateSamples(0, 0, 44100, amplitude, fade);
	}
}