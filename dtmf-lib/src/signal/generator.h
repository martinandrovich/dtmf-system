#pragma once
#include <SFML/Audio.hpp>
#include "constants.h"

class generator
{
public:
	generator();
	virtual ~generator();

	static sf::SoundBuffer*		generateSamples(int f1, int f2, int duration, int amplitude = AMPLITUDE_MAX, double fade = 0.2);
	static sf::SoundBuffer*		generateDTMF(int tone, int duration = 1000, int amplitude = AMPLITUDE_MAX, double fade = 0.2);

	// play(buffer)
	// play(int)

	// playSequence(char[])

};