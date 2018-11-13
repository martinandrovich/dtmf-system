#pragma once
#include <string>
#include <vector>
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
		void testGenerator();
		void testGeneratorSequence();
		void testDecoderLog();

		void testSampler2();

		std::map<double, short> LatencyMap(std::map< double , std::vector<short>>);

		void testDecoderKeyboardReciever();
		void testDecoderKeyboardSender();

		void testStepWindow(long long delay);
		void testLatency();
		void testLatency2();
		
		void				exportSamples(std::vector<short> &samples, std::string filename = "output_samples.dat");
		void				exportMap(std::map<double, short> map, std::string filename = "output_map.dat");
		void				plotSamples(std::vector<short> &samples, std::string filename = "samples_plot.dat");
		std::vector<short>	convertAudio(std::string filename);
		void				exportAudio(std::vector<short> &samples);

		std::string			getWorkingDirectory();
	}
}