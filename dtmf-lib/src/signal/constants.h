#pragma once

// _ //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using uint = unsigned int;

constexpr auto	PI					= 3.141592653589732;
constexpr auto	AMPLITUDE_MAX		= 16384;						// Maximum possible signal amplitude (100%) (SIGNED INT16 -> 2^15 / 2)
constexpr auto	DURATION			= 500;							// Default signal duration [ms]
constexpr auto	PAUSE				= 20;							// Default signal pause [ms]
constexpr auto	FADE				= 0.15f;						// Default signal fade percentage [decimal] (0.1 = 10%)
constexpr auto	SAMPLE_RATE			= 44100;						// Default signal sample rate in [Hz]
constexpr auto	SAMPLE_INTERVAL		= 100;							// Default interval of sample processing [mys]
constexpr auto	STEP_WINDOW_SIZE	= 5;							// Number of minimum samples in queue (signal duration / sample interval)

//const int		freqLow[4]			= {  697,  770,  852,  941 };
//const int		freqHigh[4]			= { 1209, 1336, 1477, 1633 };
const int		freq[8]				= { 697,  770,  852,  941, 1209, 1336, 1477, 1633 };
const int		freqThresholds[8]	= { 100,  100,  100,  100,  100,  100,  100,  100 };