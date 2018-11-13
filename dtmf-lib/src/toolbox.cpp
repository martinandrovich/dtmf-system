#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <functional>
#include <atomic>
#include <windows.h>

#include "dtmf/toolbox.h"

#include "signal/generator.h"
#include "signal/sampler.h"
#include "signal/processor.h"
#include "signal/decoder.h"
#include "signal/sampler2.h"

#define WINVER 0x0500

namespace dtmf
{

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace toolbox
{
	// Private Members
	
	// Private Methods
	void logPayload(uint toneId);
	void executePayload(uint toneId);
	void pressKey(int key, int pause);
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

///  Helper Methods ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Log a toneId
void toolbox::logPayload(uint toneId)
{
	std::cout << "[Payload]: " << toneId << "\n";
}

// Execute a keypress according to a toneId
void toolbox::executePayload(uint toneId)
{

	int keyMoveDuration			= 50;
	int keyPlantDuration		= 25;

	toolbox::logPayload(toneId);

	// LEFT		= 0x25
	// UP		= 0x26
	// RIGHT	= 0x27
	// DOWN		= 0x28
	// ;		= 0xBA

	// execute key press in accordance with toneID

	switch (toneId)
	{
		//Left
	case 0:
		toolbox::pressKey(0x25, keyMoveDuration);
		break;

		// Right
	case 1:
		toolbox::pressKey(0x27, keyMoveDuration);
		break;

		// Up
	case 2:
		toolbox::pressKey(0x26, keyMoveDuration);
		break;

		// Down
	case 4:
		toolbox::pressKey(0x28, keyMoveDuration);
		break;

		// Plant
	case 14:
		toolbox::pressKey(0xBA, keyPlantDuration);
		break;
	}
}

// Simulate global key press for a given duration
void toolbox::pressKey(int key, int duration)
{

	INPUT ip;

	// set up a generic keyboard event
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// press the key
	ip.ki.wVk = key; // virtual-key code for the key
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	// wait
	Sleep(duration);

	// release the key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}

// Get current working directory path; return string
std::string toolbox::getWorkingDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);

	return std::string(buffer);
}

// ...
std::map<double, short> toolbox::convertLatencyMap(std::map< double, std::vector<short>> map, double startTime)
{
	std::vector<double> time;
	std::vector<short> samples;


	std::map<double, short> output;


	double lastTime = startTime;


	for (auto const& x : map)
	{

		// vector of all samples
		for (int i = 0; i < x.second.size(); i++)
		{
			samples.push_back(x.second[i]);
		}

		double deltatime = (x.first - lastTime) / double(x.second.size());

		// vector of all time
		for (int i = 0; i < x.second.size(); i++)
		{
			time.push_back(lastTime);
			lastTime += deltatime;
		}

	}

	for (int i = 0; i < samples.size(); i++)
	{
		output[time[i]] = samples[i];
	}

	return output;
}

// ...
void toolbox::exportSamples(std::vector<short> &samples, std::string filename)
{
	std::ofstream outputStream(filename);

	for (const auto &sample : samples)
	{
		outputStream << sample << "\n";
	}

	outputStream.close();

	std::cout << "Samples array[" << samples.size() << "] exported as \"" << filename << "\" ...\n";
}

// ...
template <class key, class value>
void toolbox::exportMap(std::map<key, value> map, std::string filename)
{
	std::ofstream outputStream(filename);

	for (auto pair : map)
	{
		outputStream << pair.first << ";" << pair.second << "\n";
	}

	outputStream.close();
}

// ...
void toolbox::exportAudio(std::vector<short> &samples, std::string filename)
{
	sf::SoundBuffer buffer;
	buffer.loadFromSamples(&samples[0], samples.size(), 1, SAMPLE_RATE);

	buffer.saveToFile(filename);
}

// ...
void toolbox::plotSamples(std::vector<short> &samples, std::string filename)
{
	// export samples
	toolbox::exportSamples(samples, filename);

	// export plot function
	// or somehow include/copy the MATLAB scripts to the destination path
	// currently simply manual copy scripts folder to current working path of console-app

	std::cout << "Launching MatLab script ...\n";

	// run MATLAB script/function
	// needs to be changed to cd "/script"
	std::string cmd = "matlab -nodesktop -r \"plot_script('" + filename + "')\"";
	system(cmd.c_str());
}

// Convert an audio file to a samples array; return vector of shorts
std::vector<short> toolbox::convertAudio(std::string filename)
{
	std::cout << "Converting \"" << filename << "\" to array.\n";

	sf::SoundBuffer buffer;

	if (!buffer.loadFromFile(filename))
	{
		return { 0 };
	}

	const short* data = &buffer.getSamples()[0];
	const int size = buffer.getSampleCount();

	std::vector<short> samples(data, data + size);

	return samples;
}

///  Test Methods /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Playback all DTMF tones for 200ms
void toolbox::testGenerator()
{
	for (int i = 0; i <= 16; i++)
	{
		i = i % 16;
		generator::playback(i, 200);
	}
}

// Playback some DTMF tone sequence for 50ms each
void toolbox::testGeneratorSequence()
{
	std::vector<int> test = { 2, 0, 13, 7, 4 };
	generator::playbackSequence(test, 50);
}

// Initialize decoder and log the payload (toneId) of percieved DTMF tones
void toolbox::testDecoderLog()
{
	decoder::init(&dtmf::toolbox::logPayload);
	decoder::run();
}

// Initialize decoder and execute keypress according to the payload (toneId) of percieved DTMF tones
void toolbox::testDecoderKeyboardReciever()
{
	decoder::init(&dtmf::toolbox::executePayload);
	decoder::run();
}

// ...
void toolbox::testDecoderKeyboardSender()
{
	generator::playback(0, 50);
}

// ...
void toolbox::testStepWindow(long long delay)
{
	using namespace std::chrono;

	// variables
	steady_clock				clock;
	time_point<steady_clock>	timeStart;
	long long					timeElapsed		= 0;		// ms

	int							frequency		= 697;

	int							numToDiscard	= STEP_WINDOW_SIZE * 5;
	int							numToStore		= STEP_WINDOW_SIZE * 2;
	int							counter			= 0;

	std::vector<short>			samples;
	std::map<int, float>		goertzel;
	std::vector<short>			goertzel2(numToStore);
	sampler*					recorder;

	std::atomic<bool>			logging(false);

	// brief pause
	Sleep(1000);

	// begin
	std::cout << "Step Window Analysis, " << delay << "ms playback delay\n";

	// initialize recoder w/ lambda
	recorder = new sampler([&](std::vector<short> samplesChunk)
	{
		// check counter
		if (counter >= (numToStore + numToDiscard))
		{
			return;
		}
		
		// increment counter
		counter++;

		// discard first sample chunks
		if (counter < numToDiscard + 1)
		{
			return;
		}

		// initialize timer if sampling is beginning
		if (counter == numToDiscard + 1)
		{
			std::cout << "Timer started.\n";
			timeStart = clock.now();
			logging = true;
		}

		// log
		int		datapoint		= counter - numToDiscard -1;
		float	magnitude		= processor::goertzel(samplesChunk, frequency);

		std::cout << "Logging datapoint[" << datapoint << "][" << magnitude <<"] after " << static_cast<duration<double, std::milli>>(clock.now() - timeStart).count() << "ms.\n";
				
		goertzel[datapoint]		= magnitude;
		goertzel2[datapoint]	= magnitude;
		samples.insert(samples.end(), samplesChunk.begin(), samplesChunk.end());	

		// end
		if (counter == numToStore + numToDiscard)
		{
			logging = false;
		}

	}, true);

	// start recorder
	recorder->start(SAMPLE_RATE);

	// wait of logging start
	while (!logging) {}

	// update timer
	while (true)
	{
		timeElapsed = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

		if (timeElapsed >= delay)
		{
			std::cout << "Playing tone after " << static_cast<duration<double, std::milli>>(clock.now() - timeStart).count() << "ms \n";
			generator::playback(0, 50);
			break;
		}
	 }

	// wait until logging done
	while (logging) {}

	// stop recorder
	recorder->stop();
	
	// data here
	toolbox::plotSamples(samples, "g1.dat");
	toolbox::plotSamples(goertzel2, "g2.dat");
	toolbox::exportAudio(samples);

	// clean up
	delete recorder;
}

// ...
void toolbox::testStepWindow2(std::string args)
{

	using namespace std::chrono;

	// constants
	const int							toneDuration = 50;			// ms
	const int							latency = 100;			// ms
	long long							delay = std::stoi(args);
	const int							windowSize = SAMPLE_INTERVAL;
	const int							desiredWindows = 2 * toneDuration / SAMPLE_INTERVAL;
	const int							latencyWindows = latency / SAMPLE_INTERVAL;

	// variables
	sampler2							sampler([](std::vector<short> samples) {});
	std::thread							player;
	std::function<void()>				delayedPlayer;

	std::vector<short>					samples;
	std::map<long long, float>			goertzel;
	std::vector<short>					goertzel2(desiredWindows + latencyWindows);
	std::map<double, short>				goertzel3;

	high_resolution_clock				clock;
	time_point<high_resolution_clock>	timeStart;
	std::atomic<long long>				timeElapsed = 0;					// ms

	unsigned int						counter = 0;

	// prepare delayed playback thread
	delayedPlayer = [&]()
	{
		while (true)
		{
			timeElapsed = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

			if (timeElapsed > delay)
			{
				generator::playback(0, toneDuration);
				break;
			}
		}
	};

	// select delay from case
	;

	std::cout << "Step Window Analysis (v2), " << delay << "ms playback delay\n";
	Sleep(1000);

	// prepare sampler
	sampler.prepare();
	std::vector<short>	samplesChunk(NUMPTS);

	// start clock
	timeStart = clock.now();

	// start delayed playback thread
	player = std::thread(delayedPlayer);

	// datalogging loop
	for (int i = 0; i < (desiredWindows + latencyWindows); i++)
	{
		samplesChunk = sampler.sample();
		float	magnitude = processor::goertzel(samplesChunk, 697);
		timeElapsed = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

		//std::cout << "Logging datapoint[" << i << "][" << magnitude << "] after " << timeElapsed << "ms.\n";

		//goertzel[timeElapsed] = magnitude;
		//goertzel2[i] = magnitude;
		goertzel3[timeElapsed] = magnitude;
		//samples.insert(samples.end(), samplesChunk.begin(), samplesChunk.end());
	}

	// remove latency values
	;

	// cleanup
	player.join();

	// data here
	goertzel3;
	toolbox::exportMap(goertzel3);
}

// ...
void toolbox::testLatency()
{
	using namespace std::chrono;

	const int							delay			= 10;				// number of chunks to trash
	const int							dur				= delay + 100;		// number of chunks to log

	// variables
	high_resolution_clock				clock;
	time_point<high_resolution_clock>	timeStart;
	long long							timeElapsed		= 0;		// ms

	bool								initialized		= false;
	bool								playing			= false;
	std::atomic<bool>					logging			= false;
	std::atomic<int>					counter			= 0;
	double								begin;

	std::map<double, short>				logEvents;
	std::map<double, std::vector<short>>logChunks;
	std::vector<short>					samples;

	sampler2	recorder([&](std::vector<short> samplesChunk)
	{
		logging = true;

		counter++;

		// trash until delay is reached
		if (counter < delay)
		{
			return;
		}
		else if (!playing)
		{
			logEvents[timeElapsed] = 5000;
			playing = true;
			begin = timeElapsed;
			generator::playback(0, 100, true);
			logEvents[timeElapsed] = 5001;
			return;
		}

		logEvents[timeElapsed] = 500;
		logChunks[timeElapsed] = samplesChunk;

		samples.insert(samples.end(), samplesChunk.begin(), samplesChunk.end());
		
		logging = false;
	}, true);

	// init clock
	timeStart = clock.now();

	// loop
	while (true)
	{
		timeElapsed = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

		if (counter > dur) { break; }

		if (!initialized)
		{
			logEvents[timeElapsed] = 1000;
			initialized = true;
			recorder.start();
		}
		else if (!logging && logEvents.count(timeElapsed) == 0)
		{
			logEvents[timeElapsed] = 0;
		}
	}

	recorder.stop();
	Sleep(1000);

	// data here
	auto logFinal = toolbox::LatencyMap(logChunks, begin);
	toolbox::exportMap(logFinal);
	toolbox::exportAudio(samples);
	toolbox::plotSamples(samples);
	logEvents;

}

// Test sampler2 class; record a single chunk using callback
void toolbox::testSampler2()
{
	sampler2* test = new sampler2([](std::vector<short> samples) {std::cout << "Got chunk [" << samples.size() << "]\n"; });
	test->prepare();

	auto chunk = test->sample();
	//toolbox::exportAudio(chunk);
	test->stop();
	delete test;
}

}