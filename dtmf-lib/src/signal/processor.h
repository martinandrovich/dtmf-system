#pragma once
#include <string>
#include <vector>
#include <array>
#include <valarray>
#include <ccomplex>
#include "constants.h"

using cArray = std::valarray<std::complex<double>>;
//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

namespace processor
{
	// Public Members
	;

	// Public Methods
	void					equalize();

	void					fft(cArray& x);
	cArray					fft(std::vector<short>& samples);
	void					fft2(cArray& x);
	cArray					fft2(std::vector<short>& samples);

	void					hanningWindow(std::vector<short> &samples);

	float					goertzel(std::vector<short> &samples, int frequency);
	std::array<float, 8>	goertzelArray(std::vector<short> &samples);

	void					printGoertzelArray(std::array<float, 8> &sampleArray);
	float					getAverageAmplitude(std::array<float, 8> &sampleArray);
}