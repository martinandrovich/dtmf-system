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

		void testDecoderKeyboardReciever();
		void testDecoderKeyboardSender();
		
		void				exportSamples(std::vector<short> &samples, std::string filename = "output");
		void				plotSamples(std::vector<short> &samples);
		std::vector<short>	convertAudio(std::string filename);

		std::string			getWorkingDirectory();
	}
}