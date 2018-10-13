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

	sf::SoundBuffer*			generateSamples(uint f1, uint f2, uint duration = DURATION, uint amplitude = AMPLITUDE_MAX, float fadePercentage = FADE, uint sampleRate = SAMPLE_RATE);
	sf::SoundBuffer*			generateDTMF(uint tone, uint duration = DURATION, uint amplitude = AMPLITUDE_MAX, float fadePercentage = FADE, uint sampleRate = SAMPLE_RATE);

	void						playback(uint tone, uint duration);
	void						playbackSequence(std::vector<int> &sequence, int duration = DURATION, int pause = PAUSE);

	static sf::Sound*			player = new sf::Sound;
};