#pragma once
#include <SFML/Audio.hpp>
#include "constants.h"

class generator
{
public:
	generator();
	virtual ~generator();

	static sf::SoundBuffer*		generateSamples(int f1, int f2, int fs, int amplitude = 15000, double fade = 5000.f);
	static sf::SoundBuffer*		generateDTMF(char tone, int amplitude = 15000, double fade = 5000.f);

	// duration

	// play(buffer)
	// play(char)

	// playSequence(char[])

};