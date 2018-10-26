#include <iostream>
#include <dtmf.h>

int main()
{	
	
	std::cout << "Ear Rape Simulator 1.0\n";

	std::cout << "Testing Decoder/Sampler\n";
	//dtmf::testGenerator();
	dtmf::testDecodeSample();
	//dtmf::testGeneratorSequence();
	
	std::cin.get();
	return 0;

}