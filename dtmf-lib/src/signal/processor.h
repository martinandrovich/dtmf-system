#pragma once
#include <string>
#include <vector>
#include <array>

#include "constants.h"

//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

namespace processor
{
	// Public Members
	;

	// Public Methods
	void equalize();
	void fft();

	void					hanningWindow(std::vector<short> &samples);
	float					goertzel(std::vector<short> &samples, int frequency);
	std::array<float, 8>	goertzelArray(std::vector<short> &samples);

	void					printGoertzelArray(std::array<float, 8> &sampleArray);
	float					getAverageAmplitude(std::array<float, 8> &sampleArray);
}