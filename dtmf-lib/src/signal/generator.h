#pragma once
#include <SFML/Audio.hpp>
#include "constants.h"

// Namespaces vs. Static Class (when no object is needed)
// https://stackoverflow.com/questions/9321/how-do-you-create-a-static-class-in-c

// Static?
// https://www.youtube.com/watch?v=f3FVU-iwNuA && https://www.youtube.com/watch?v=V-BFlMrBtqQ

namespace generator
{
	using uint = unsigned int;

	sf::SoundBuffer*			generateSamples(uint f1, uint f2, uint duration = 500, uint amplitude = AMPLITUDE_MAX, float fadePercentage = 0.2f, uint sampleRate = SAMPLE_RATE);
	sf::SoundBuffer*			generateDTMF(uint tone, uint duration = 500, uint amplitude = AMPLITUDE_MAX, float fadePercentage = 0.2f, uint sampleRate = SAMPLE_RATE);

	void						playback(uint tone, uint duration);
	void						playbackSequence(uint sequence[], uint pause = 50);

	static sf::Sound*			player = new sf::Sound;
};