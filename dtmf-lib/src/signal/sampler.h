#pragma once
#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>
#include <vector>
#include <functional>

#include "constants.h"

class sampler : public sf::SoundRecorder
{
//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////
public:

	// Constructor & Destructor
	sampler(std::function<void(std::vector<short> samples)> callback);
	virtual ~sampler();

	// Status Enum
	enum class state {
		unitialized,
		idle,
		sampling,
		processing
	};

	state	getStatus();

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	int					rate;
	int					interval;
	state				status;
	std::vector<short>	buffer;

	//void(*callback)(std::vector<short> samples);
	std::function<void(std::vector<short>)> callback;

//// Protected Declarations ///////////////////////////////////////////////////////////////////////////////////////////////////////
protected:
	virtual bool onStart() override;
	virtual void onStop() override;
	virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount) override;
};