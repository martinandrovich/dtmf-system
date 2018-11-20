#pragma once

// _ //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using			uint				= unsigned int;
using			uchar				= unsigned char;

constexpr auto 	DATA_PATH			= "dat/";										// Path of data directory (default "dat/")
constexpr auto	PI					= 3.141592653589732;
constexpr auto	AMPLITUDE_MAX		= 16384;										// Maximum possible signal amplitude (100%) (SIGNED INT16 -> 2^15 / 2)
constexpr auto	DURATION			= 50;											// Default signal duration [ms]
constexpr auto	PAUSE				= 50;											// Default signal pause [ms]
constexpr int	DEBOUNCE			= (DURATION + 20) * 1;							// Default debounce value [ms] (default = 90% of DURATION + PAUSE)
constexpr int	LATENCY				= 100;											// It's the fucking latency, bitch
constexpr auto	FADE				= 0.10f;										// Default signal fade percentage [decimal] (0.1 = 10%)
constexpr auto	SAMPLE_RATE			= 88200;										// Default signal sample rate in [Hz] (default = 44100)
constexpr auto	SAMPLE_INTERVAL		= 10;											// Default interval of sample processing [ms]
constexpr auto	STEP_WINDOW_SIZE	= 5;											// Number of minimum samples in queue (signal duration / sample interval)
constexpr int 	NUMPTS				= SAMPLE_RATE * (SAMPLE_INTERVAL / 1000.f);		// Number of samples (size of array for sampler2)

//const int		freqLow[4]			= {  697,  770,  852,  941 };
//const int		freqHigh[4]			= { 1209, 1336, 1477, 1633 };
const int		freq[8]				= { 697,  770,  852,  941, 1209, 1336, 1477, 1633 };
const int		N[8]				= { 441,  403,  267,  441,  294,  331,  441,  441 };
const int		freqThresholds[8]	= { 50,  50,  50,  50,  100,  100,  100,  100 };