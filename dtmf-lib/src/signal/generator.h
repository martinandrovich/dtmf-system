#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <chrono>

#include "constants.h"

namespace generator
{
	using namespace std::chrono;
	
	// Public Members
	enum class state {
		idle,
		playing
	};

	// Public Methods
	sf::SoundBuffer*					generateSamples(uint f1,uint f2, uint duration = DURATION,
														uint amplitude = AMPLITUDE_MAX, float fadePercentage = FADE, uint sampleRate = SAMPLE_RATE);

	sf::SoundBuffer*					generateDTMF(uint tone, uint duration = DURATION,
													 uint amplitude = AMPLITUDE_MAX, float fadePercentage = FADE, uint sampleRate = SAMPLE_RATE);
		
	void								playback(uint tone, uint duration = DURATION, bool parallel = false , bool fade = true);
	void								playbackSequence(std::vector<int> &sequence, int duration = DURATION, int pause = PAUSE);

	state								getState();
	time_point<high_resolution_clock>	getTimestamp();
};