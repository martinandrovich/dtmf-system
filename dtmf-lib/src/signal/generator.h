#pragma once
#include <SFML/Audio.hpp>
#include "constants.h"
class generator
{

	using uint = unsigned int;

public:
	
	static sf::SoundBuffer*		generateSamples(uint f1, uint f2, uint duration = 500, uint amplitude = AMPLITUDE_MAX, float fadePercentage = 0.2f, uint sampleRate = SAMPLE_RATE);
	static sf::SoundBuffer*		generateDTMF(uint tone, uint duration = 500, uint amplitude = AMPLITUDE_MAX, float fadePercentage = 0.2f, uint sampleRate = SAMPLE_RATE);

	static void					playback(uint tone, uint duration);
	static void					playbackSequence(uint sequence[], uint pause = 50);

private:
	static sf::Sound*			player;

	// Disallow object creation
	generator() = delete;
};