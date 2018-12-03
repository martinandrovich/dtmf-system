#pragma once
#include <valarray>
#include <complex>

// _ //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using			uint				= unsigned int;
using			uchar				= unsigned char;
using			cArray				= std::valarray<std::complex<double>>;

constexpr auto 	DATA_PATH			= "dat/";										// Path of data directory (default "dat/")
constexpr auto	PI					= 3.141592653589732;
constexpr auto	AMPLITUDE_MAX		= 32767;										// Maximum possible signal amplitude (100%) (SIGNED INT16 -> � 2^15 - 1)
constexpr auto	DURATION			= 50;											// Default signal duration [ms]
constexpr auto	PAUSE				= 20;											// Default signal pause [ms]
constexpr int	DEBOUNCE			= (DURATION + 40) * 1;							// Default debounce value [ms] (default = 90% of DURATION + PAUSE)
constexpr int	DEBOUNCE2			= DURATION;										// Default debounce value [ms] (default = DURATION)
constexpr auto	TH_MULTIPLIER_H		= 1.0;											// Threshold multiplier second time
constexpr auto	TH_MULTIPLIER_L		= TH_MULTIPLIER_H * 0.4;						// Threshold multiplier second time
constexpr auto	TH_LEVELER			= 0.67;											// Threshold leveler
constexpr int	TIMEOUT				= 500;											// State Machine Protocol
constexpr int	LATENCY				= 100;											// It's the fucking latency, bitch
constexpr int	LATENCY_BUFFER		= 0;											// It's the fucking latency buffer, bitch
constexpr auto	FADE				= 0.10f;										// Default signal fade percentage [decimal] (0.1 = 10%)
constexpr auto	SAMPLE_RATE			= 44100;										// Default signal sample rate in [Hz] (default = 44100)
constexpr auto	SAMPLE_INTERVAL		= 10;											// Default interval of sample processing [ms]
constexpr auto	STEP_WINDOW_SIZE	= 5;											// Number of minimum samples in queue (signal duration / sample interval)
constexpr int 	NUMPTS				= SAMPLE_RATE * (SAMPLE_INTERVAL / 1000.f);		// Number of samples (size of array for sampler2)

//const int		freqLow[4]			= {  697,  770,  852,  941 };
//const int		freqHigh[4]			= { 1209, 1336, 1477, 1633 };
const int		freq[8]				= {  697,	  770,	  852,	  941,	 1209,	 1336,	 1477,	 1633 };
//static int		freqThresholds[8]	= {   75,	   75,	   75,	   75,	  100,	  100,	  100,	  100 };
static int		freqThresholds[8]	= {   63,	   97,	   281,	   277,	  282,	  241,	  279,	  346 };
const float		freqMultiplier[8]	= { 1.00,	 1.00,	  1.00,  1.00,	 1.00,	 1.00,	 1.00,	 1.00 };

//| 697 Hz = 307.878 | 770 Hz = 258.465 | 852 Hz = 426.028 | 941 Hz = 623.004 | 1209 Hz = 881.878 | 1336 Hz = 625.134 | 1477 Hz = 574.286 | 1633 Hz = 369.555 |
