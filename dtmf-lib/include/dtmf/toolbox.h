#pragma once
#include <string>
#include <vector>
#include <array>
#include <map>

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
		void									testGenerator();
		void									testGeneratorSequence();
		void									testDecoderLog();

		void									testSampler2();

		std::map<double, short>					convertLatencyMap(std::map< double, std::vector<short>> map, double startTime);

		void									testDecoderKeyboardReciever();
		void									testDecoderKeyboardSender();

		void									testStepWindow(long long delay);
		void									testStepWindow2(std::string args);
		void									testLatency();
		
		
		void									exportSamples(std::vector<short> &samples, std::string filename = "output_samples.dat");
		template <class key, class value> void	exportMap(std::map<key, value> map, std::string filename = "output_map.dat");
		void									exportAudio(std::vector<short> &samples, std::string filename = "output_sound.wav");
		void									plotSamples(std::vector<short> &samples, std::string filename = "samples_plot.dat", std::array<std::string, 3> labels = { "Samples Plot", "x-axis", "y-axis" });
		template <class key, class value> void	plotMap(std::map<key, value> &map, std::string filename = "map_plot.dat", std::array<std::string, 3> labels = { "Map Plot", "x-axis", "y-axis" });
		
		std::vector<short>						convertAudio(std::string filename);

		std::string								getWorkingDirectory();
	}
}