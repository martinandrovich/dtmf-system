#include <string>
#include <vector>
#include <array>
#include <iostream>

#include "processor.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace processor
{
	// Private Members
	;

	// Private Methods
	;
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Get average magnitue of Goertzel array; return float
float processor::getAverageAmplitude(std::array<float, 8> &sampleArray)
{
	float sum = 0.f;

	for (const auto& s : sampleArray)
	{
		sum += s;
	}		

	return sum/8.f;
}

// Run goertzel algorithm on chunk of samples for a specific frequency; return float
float processor::goertzel(std::vector<short> &samples, int frequency)
{
	//float   omega, sine, cosine, coeff, w0, w1, w2, magnitude, real, imag;

	//Difference equation for Goertzel:
		//N = numSamples
		//k = (N*targetFreq)/sampleFreq
	//w(n)=2cos(2*pi*k/N)*w(n-1)-w(n-2)+x(n)
	

	// variable definitions
	int		numSamples			= samples.size();
	float   numSamplesFloat		= (float)numSamples;
	float   scalingFactor		= numSamples / 2.f;
	int		k					= (int)(0.5 + ((numSamplesFloat * frequency) / SAMPLE_RATE));

	float	omega				= (2.f * PI * k) / numSamplesFloat;
	float	sine				= sin(omega);
	float	cosine				= cos(omega);

	//Difference equation with new omega:
	//w(n)=2cos(omega)*w(n-1)-w(n-2)+x(n)

	float	w0					= 0;
	float	w1					= 0;
	float	w2					= 0;

	//w0 =w(n)
	//w1 =w(n-1)
	//w2 =w(n-2)

	for (const auto &s : samples)
	{
		w0 = (2.f * cosine ) * w1 - w2 + s;
		w2 = w1;
		w1 = w0;
	}

	// calculate the real and imaginary results + magnitude
	// scaling appropriately
	float	real				= (w1 - w2 * cosine) / scalingFactor;
	float	imag				= (w2 * sine) / scalingFactor;
	float	magnitude			= sqrtf(real * real + imag * imag);

	// return the magnitude
	return magnitude;
}

// Run goertzel algorithm on chunk of samples for all DTMF frequncies; return array[8] of magnitudes (float)
std::array<float, 8> processor::goertzelArray(std::vector<short> &samples)
{
	std::array<float, 8> tempArray;

	for (int i = 0; i < 8; i++)
	{
		tempArray[i] = goertzel(samples, freq[i]);
	}

	return tempArray;
}

// Print magnitudes of goertzelArray
void processor::printGoertzelArray(std::array<float, 8> &sampleArray)
{
	std::cout << "Goertzel array:\n";

	for (int i = 0; i < 8; i++)
	{
		std::cout << freq[i] << " : " << sampleArray[i] << " | ";
	}

	std::cout << "\n\n";
}