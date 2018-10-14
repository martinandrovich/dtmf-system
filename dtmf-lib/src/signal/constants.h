#pragma once

// _ //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr auto PI				= 3.141592653589732;
constexpr auto DURATION			= 500;							// Default signal duration in ms
constexpr auto PAUSE			= 20;							// Default signal pause in ms
constexpr auto FADE				= 0.15f;						// Default signal fade percentage in decimal (0.1 = 10%)
constexpr auto SAMPLE_RATE		= 44100;						// Default signal sample rate in Hz
constexpr auto AMPLITUDE_MAX	= 16384;						// Maximum possible signal amplitude (100%) (SIGNED INT16 -> 2^15 / 2)