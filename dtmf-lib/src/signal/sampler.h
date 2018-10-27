#pragma once
#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>

#include "constants.h"

class sampler : public sf::SoundRecorder
{
//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////
public:

	// Constructor & Destructor
	sampler(void(*callback)(std::vector<short> samples));
	virtual ~sampler();

	// Status Enum
	enum class state {
		unitialized,
		idle,
		sampling,
		processing
	};

	void	run();
	state	getStatus();

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	int					rate;
	int					interval;
	state				status = state::unitialized;
	std::vector<short>	buffer;

	void(*callback)(std::vector<short> samples);

//// Protected Declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////
protected:
	virtual bool onStart() override;
	virtual void onStop() override;
	virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override;
};