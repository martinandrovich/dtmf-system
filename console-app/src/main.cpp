#include <iostream>
#include <dtmf.h>

int main()
{	
	
	std::cout << "Ear Rape Simulator 1.0\n";

	std::cout << "Testing Generator Sequence\n";
	dtmf::testSamplerDecoder();
	dtmf::testGeneratorSequence();
	
	std::cin.get();
	return 0;

}