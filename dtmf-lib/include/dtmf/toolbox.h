#pragma once
#include <string>
#include <vector>

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

		void testDecoderKeyboardReciever();
		void testDecoderKeyboardSender();

		void testStepWindow(long long delay);
		void testLatency();
		
		void				exportSamples(std::vector<short> &samples, std::string filename = "output.dat");
		void				plotSamples(std::vector<short> &samples, std::string filename = "samples_plot.dat");
		std::vector<short>	convertAudio(std::string filename);
		void				exportAudio(std::vector<short> &samples);

		std::string			getWorkingDirectory();
	}
}