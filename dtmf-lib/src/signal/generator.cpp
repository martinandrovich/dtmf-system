#pragma once
#include <Windows.h>

#include "generator.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace generator
{
	// Private Members
	sf::Sound*			player = new sf::Sound;
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Generate an array of samples from two given frequencies; return a pointer to a heap-allocated SFML SoundBuffer object
sf::SoundBuffer* generator::generateSamples(uint f1, uint f2, uint duration, uint amplitude, float fadePercentage, uint sampleRate)
{
	// Variables
	float					sampleSize = sampleRate * (duration / 1000.f);
	float					fadeSize = fadePercentage * sampleSize;
	std::vector<sf::Int16>	temp(sampleSize);
	uint					amplitudeFinal;

	// Create NEW SoundBuffer on HEAP
	sf::SoundBuffer*		buffer = new sf::SoundBuffer;						// [NEEDS GARBAGE COLLECTION !!!]

	// Generate sample array
	for (uint i = 0; i < sampleSize; i++)
	{
		// Fade function on amplitude
		if (i < fadeSize)
		{
			amplitudeFinal = (i / (fadeSize * 1.f)) * amplitude;
		}
		else if (i > sampleSize - fadeSize)
		{
			amplitudeFinal = ((sampleSize - i) / (fadeSize * 1.f)) * amplitude;
		}
		else
		{
			amplitudeFinal = amplitude;
		}

		// Array entry
		temp[i] = amplitudeFinal * (sin(f1 * 2 * PI*(i * 1.f / sampleRate)) + sin(f2 * 2 * PI*(i * 1.f / sampleRate)));
	}

	// Load and return buffer
	buffer->loadFromSamples(&temp[0], sampleSize, 1, sampleRate);
	return buffer;
}

// Generate an array of samples from a given tone (char 0-D); return a pointer to a heap-allocated SFML SoundBuffer object
sf::SoundBuffer* generator::generateDTMF(uint tone, uint duration, uint amplitude, float fadePercentage, uint sampleRate)
{
	// Check if valid tone input
	if (tone > 15)
		return nullptr;

	// Lists of frequencies
	int freqLow[4]	= {  697,  770,  852,  941 };
	int freqHigh[4] = { 1209, 1336, 1477, 1633 };

	// Return buffer generated from an appropriate set of frequencies
	return generator::generateSamples(freqLow[tone / 4], freqHigh[tone % 4], duration, amplitude);

}

// Playback a tone for a duration; spinlock while playing
void generator::playback(uint tone, uint duration)
{
	// Create buffer
	auto buffer = generateDTMF(tone, duration);

	// Set buffer and play
	player->setBuffer(*buffer);
	player->play();
	
	// Wait while playing
	while (player->getStatus() == player->Playing)
		;

	// Cleanup
	delete buffer;
}

// Playback a sequence of tones for a duration and pause between; spinlock while playing
void generator::playbackSequence(std::vector<int> &sequence, int duration, int pause)
{
	for (auto i : sequence)
	{
		playback(i, duration);
		Sleep(pause); // BAD IMPLEMENTATION !!!!
	}
		
}

// Generate a buffer of samples to a vector (sf::SoundBuffer<Int16> -> std::vector<short>)
std::vector<short> convertBuffer(sf::SoundBuffer& buffer)
{
	const short* data = &buffer.getSamples()[0]; // Int16*
	const int size = buffer.getSampleCount();

	std::vector<sf::Int16> samples(data, data + size);
	//std::vector<sf::Int16> samples((const short*)&buffer.getSamples()[0], (const short*)&buffer.getSamples()[0] + (int)buffer.getSampleCount());

	return samples;
}