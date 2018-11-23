#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <atomic>
#include <windows.h>

#include "generator.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace generator
{
	// Private Members
	sf::Sound*										player = new sf::Sound;
	std::atomic<state>								status;

	high_resolution_clock							clock;
	std::atomic<time_point<high_resolution_clock>>	lastPlayed;
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Generate an array of samples from two given frequencies; return a pointer to a heap-allocated SFML SoundBuffer object
sf::SoundBuffer* generator::generateSamples(uint f1, uint f2, uint duration, uint amplitude, float fadePercentage, uint sampleRate)
{
	// variables
	int						sampleSize = sampleRate * (duration / 1000.f);
	float					fadeSize = fadePercentage * sampleSize;
	std::vector<sf::Int16>	temp(sampleSize);
	uint					amplitudeFinal;

	// create heap allocated sf::SoundBuffer
	sf::SoundBuffer*		buffer = new sf::SoundBuffer;						// [NEEDS GARBAGE COLLECTION !!!]

	// generate sample array
	for (uint i = 0; i < sampleSize; i++)
	{
		// linear fade function on amplitude
		if (i < fadeSize)
		{
			amplitudeFinal = (i / (fadeSize * 1.f)) * amplitude;
			//amplitudeFinal = amplitude;
		}
		else if (i > sampleSize - fadeSize)
		{
			amplitudeFinal = ((sampleSize - i) / (fadeSize * 1.f)) * amplitude;
		}
		else
		{
			amplitudeFinal = amplitude;
		}

		// array entry
		temp[i] = amplitudeFinal * (sin(f1 * 2 * PI * (i * 1.f / sampleRate)) + sin(f2 * 2 * PI * (i * 1.f / sampleRate)));
	}

	// load and return buffer
	buffer->loadFromSamples(&temp[0], sampleSize, 1, sampleRate);
	return buffer;
}

// Generate an array of samples from a given tone (char 0-D); return a pointer to a heap-allocated SFML SoundBuffer object
sf::SoundBuffer* generator::generateDTMF(uint toneID, uint duration, uint amplitude, float fadePercentage, uint sampleRate)
{
	// check if valid tone input
	if (toneID > 15)
	{
		return nullptr;
	}		

	// return buffer generated from an appropriate set of frequencies
	return generator::generateSamples(freq[toneID / 4], freq[(toneID % 4) + 4], duration, amplitude);
	///return generator::generateSamples(freqLow[toneID / 4], freqHigh[toneID % 4], duration, amplitude);
}

// Playback a tone for a duration; spinlock while playing
void generator::playback(uint toneId, uint duration, bool parallel)
{
	// check toneId
	if (toneId < 0 || toneId > 15)
	{
		std::cout << "Wrong toneId you dumb fucks.\n";
		return;
	}
	
	// create buffer
	auto buffer = generateDTMF(toneId, duration);

	// update status
	generator::status = generator::state::playing;

	// set buffer and play
	generator::player->setBuffer(*buffer);
	generator::player->play();

	// update timestamp
	generator::lastPlayed = generator::clock.now();
	
	// log
	std::cout << "Playing tone [" << toneId << " | " << duration << "ms]\n";

	// escape function if parellel enabled			!!! BAD FOR MEMORY // ONLY FOR DEBUGGING
	if (parallel)
	{
		generator::status = generator::state::idle;
		return;
	}

	// block thread while playing (parallel = false)
	while (generator::player->getStatus() == generator::player->Playing)
	{
		;
	}
	
	// update status
	generator::status = generator::state::idle;

	// cleanup
	delete buffer;
}

// Playback a sequence of tones for a duration and pause between; block thread while playing
void generator::playbackSequence(std::vector<int> &sequence, int duration, int pause)
{
	for (const auto &tone : sequence)
	{
		generator::playback(tone, duration);
		std::this_thread::sleep_for(std::chrono::milliseconds(pause));
	}
}

// Return status state; thread safe
generator::state generator::getState()
{
	return generator::status;
}

// Return timestamp of lastPlayed
generator::time_point<generator::high_resolution_clock>	generator::getTimestamp()
{
	return generator::lastPlayed;
}