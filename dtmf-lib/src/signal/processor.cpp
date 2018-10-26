#pragma once
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
float getAverageAmplitude(std::array<float, 8> &sampleArray)
{
	float sum = 0.f;

	for (const auto& s : sampleArray)
		sum += s;

	return sum/8.f;
}

void processor::printGoertzelArray(std::array<float, 8> &sampleArray)
{
	std::cout << "Goertzel array:\n";

	for (int i = 0; i < 8; i++)
		std::cout << freq[i] << " : " << sampleArray[i] << " | ";

	std::cout << "\n\n";
}

// Run Goertzel algorithm on chunk of samples for a specific frequency; return float
float processor::goertzel(std::vector<short> &samples, int frequency)
{
	int     k, i;
	float   floatnumSamples;
	float   omega, sine, cosine, coeff, q0, q1, q2, magnitude, real, imag;
	int		numSamples = samples.size();

	float   scalingFactor = numSamples / 2.f;

	floatnumSamples = (float)numSamples;
	k = (int)(0.5 + ((floatnumSamples * frequency) / SAMPLE_RATE));
	omega = (2.f * PI * k) / floatnumSamples;
	sine = sin(omega);
	cosine = cos(omega);
	coeff = 2.0 * cosine;
	q0 = 0;
	q1 = 0;
	q2 = 0;

	for (const auto &s : samples)
	{
		q0 = coeff * q1 - q2 + s;
		q2 = q1;
		q1 = q0;
	}

	// calculate the real and imaginary results
	// scaling appropriately
	real = (q1 - q2 * cosine) / scalingFactor;
	imag = (q2 * sine) / scalingFactor;

	return sqrtf(real*real + imag * imag);
}

std::array<float, 8> processor::goertzelArray(std::vector<short> &samples)
{
	std::array<float, 8> tempArray;

	for (int i = 0; i < 8; i++)
	{
		tempArray[i] = goertzel(samples, freq[i]);
	}

	return tempArray;
}

// Run Goertzel for all DTMF frequncies ... !!!
std::array<int, 2> processor::getDTMFPositions(std::vector<short> &samples)
{
	std::array<float, 8> tempArray;

	float freqLow = 0;
	float freqHigh = 0;

	int indexHigh = -1;
	int indexLow = -1;

	// Iterate all possible DTMF frequencies
	for (int i = 0; i < 8; i++)
	{
		// get magnitude for freq[i]
		tempArray[i] = goertzel(samples, freq[i]);

		if (i < 4 && tempArray[i] > freqThresholds[i] && tempArray[i] > freqLow)
		{
			freqLow = tempArray[i];
			indexLow = i;
		}
		else if (i >= 4 && tempArray[i] > freqThresholds[i] && tempArray[i] > freqHigh)
		{
			freqHigh = tempArray[i];
			indexHigh = (i - 4);
		}
			
	}

	std::array<int, 2> data = { indexLow, indexHigh };

	return data;
}
