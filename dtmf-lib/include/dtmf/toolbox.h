#pragma once
#include <string>
#include <vector>
#include <array>
#include <map>

#include "..\src\signal\constants.h"

//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

namespace dtmf
{
	namespace toolbox
	{

		// Public Constructs
		;

		// Public Members
		;

		// Public Methods
		
		void									printPayload(uint toneId);
		void									executePayload(uint toneId);

		void									playbackSequence(std::string args);

		void									testGenerator();
		void									testSampler();

		void									logDecoder();
		void									logDecoderQuiet();
		void									logSequence();
		void									logGoertzel(std::string args);
		void									logGoertzelAverage(std::string args);

		void									testDecoderKeyboardReciever();
		void									testDecoderKeyboardSender();

		void									testStepWindow(std::string args);

		void									testLatency();
		void									testLatency2();
		
		void									testGeneratorGoertzel(std::string args);
		void									testGoertzel(std::string args);
		
		void									exportSamples(std::vector<short> &samples, std::string filename = "output_samples.dat");
		template <class key, class value> void	exportMap(std::map<key, value> map, std::string filename = "output_map.dat");
		void									exportAudio(std::vector<short> &samples, std::string filename = "output_sound.wav");
		void									plotSamples(std::vector<short> &samples, std::string filename = "samples_plot.dat", std::array<std::string, 3> labels = { "Samples Plot", "x-axis", "y-axis" });
		template <class key, class value> void	plotMap(std::map<key, value> &map, std::string filename = "map_plot.dat", std::array<std::string, 3> labels = { "Map Plot", "x-axis", "y-axis" });
		
		std::vector<short>						convertAudio(std::string filename);
		std::map<double, short>					convertLatencyMap(std::map< double, std::vector<short>> map, double startTime);

		std::string								getWorkingDirectory();
	}
}