#pragma once

// _ //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using uint = unsigned int;

constexpr auto PI					= 3.141592653589732;
constexpr auto DURATION				= 500;							// Default signal duration [ms]
constexpr auto PAUSE				= 20;							// Default signal pause [ms]
constexpr auto FADE					= 0.15f;						// Default signal fade percentage [decimal] (0.1 = 10%)
constexpr auto SAMPLE_RATE			= 44100;						// Default signal sample rate in [Hz]
constexpr auto SAMPLE_INTERVAL		= 50;							// Default interval of sample processing [ms]
constexpr auto AMPLITUDE_MAX		= 16384;						// Maximum possible signal amplitude (100%) (SIGNED INT16 -> 2^15 / 2)