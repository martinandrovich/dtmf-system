#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <chrono>
#include <functional>
#include <atomic>
#include <windows.h>
#include <SFML/Audio.hpp>

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
	void pressKey(int key, int pause);
	
	void makeDataDirectory();
	std::vector<short>	convertSFBuffer(sf::SoundBuffer& buffer);
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

///  Helper Methods ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Log a toneId
void toolbox::printPayload(uint toneId)
{
	std::cout << "[Payload]: " << toneId << "\n";
}

// Execute a keypress according to a toneId
void toolbox::executePayload(uint toneId)
{

	int keyMoveDuration			= 250;
	int keyPlantDuration		= 25;

	toolbox::printPayload(toneId);

	// LEFT		= 0x25
	// UP		= 0x26
	// RIGHT	= 0x27
	// DOWN		= 0x28
	// ;		= 0xBA

	// execute key press in accordance with toneId

	switch (toneId)
	{
		//Left
	case 1:
		toolbox::pressKey(0x25, keyMoveDuration);
		break;

		// Right
	case 4:
		toolbox::pressKey(0x27, keyMoveDuration);
		break;

		// Up
	case 2:
		toolbox::pressKey(0x26, keyMoveDuration);
		break;

		// Down
	case 3:
		toolbox::pressKey(0x28, keyMoveDuration);
		break;

		// Plant
	case 5:
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
void toolbox::makeDataDirectory()
{
	//system(std::string("mkdir \"" + std::string(DATA_PATH) + "\"").c_str());
	CreateDirectory(DATA_PATH, NULL);
}

// ...
std::map<double, short> toolbox::convertLatencyMap(std::map< double, std::vector<short>> map, double startTime)
{
	std::vector<double>			time;
	std::vector<short>			samples;
	std::map<double, short>		output;
	double						lastTime = startTime;
	
	// split map into time & samples
	for (auto const& x : map)
	{
		// vector of all samples
		for (int i = 0; i < x.second.size(); i++)
		{
			samples.push_back(x.second[i]);
		}

		// update deltaTime
		double deltatime = (x.first - lastTime) / double(x.second.size());

		// vector of all time
		for (int i = 0; i < x.second.size(); i++)
		{
			time.push_back(lastTime);
			lastTime += deltatime;
		}
	}

	// create output map
	for (int i = 0; i < samples.size(); i++)
	{
		output[time[i]] = samples[i];
	}

	return output;
}

// Export a vector of shorts as a data file
void toolbox::exportSamples(std::vector<short> &samples, std::string filename)
{
	// create data directory & update filename
	toolbox::makeDataDirectory();
	filename = DATA_PATH + filename;

	std::ofstream outputStream(filename);

	for (const auto &sample : samples)
	{
		outputStream << sample << "\n";
	}

	outputStream.close();

	std::cout << "Samples array[" << samples.size() << "] exported as \"" << filename << "\" ...\n";
}

// Export a templated map as a data file
template <class key, class value>
void toolbox::exportMap(std::map<key, value> map, std::string filename)
{
	// create data directory & update filename
	toolbox::makeDataDirectory();
	filename = DATA_PATH + filename;

	std::ofstream outputStream(filename);

	/* C++ 17
	for (auto const&[k, v] : map)
	{
		std::cout << k << ";" << v << "\n";
	}
	*/

	for (auto pair : map)
	{
		outputStream << pair.first << ";" << pair.second << "\n";
	}

	outputStream.close();

	std::cout << "The map[" << map.size() << "] exported as \"" << filename << "\" ...\n";
}

// Export a vector of shorts as an audio file
void toolbox::exportAudio(std::vector<short> &samples, std::string filename)
{
	// create data directory & update filename
	toolbox::makeDataDirectory();
	filename = DATA_PATH + filename;

	sf::SoundBuffer buffer;
	buffer.loadFromSamples(&samples[0], samples.size(), 1, SAMPLE_RATE);

	buffer.saveToFile(filename);
}

// Plot vector of shorts using MATLAB
void toolbox::plotSamples(std::vector<short> &samples, std::string filename, std::array<std::string, 3> labels)
{
	// export samples
	toolbox::exportSamples(samples, filename);

	// export plot function
	// or somehow include/copy the MATLAB scripts to the destination path
	// currently simply manual copy scripts folder to current working path of console-app

	std::cout << "Launching MatLab script ...\n";

	// run MATLAB script/function
	// needs to be changed to cd "/script"
	std::string cmd = "matlab -nodesktop -r \"plot_samples('" + std::string(DATA_PATH) + filename + "', '" + labels[0] + "', '" + labels[1] + "', '" + labels[2] + "')\"";
	system(cmd.c_str());
}

// Plot templated map using MATLAB
template <class key, class value>
void toolbox::plotMap(std::map<key, value> &map, std::string filename, std::array<std::string, 3> labels)
{
	// export samples
	toolbox::exportMap(map, filename);

	// export plot function
	// or somehow include/copy the MATLAB scripts to the destination path
	// currently simply manual copy scripts folder to current working path of console-app

	std::cout << "Launching MatLab script ...\n";

	// run MATLAB script/function
	// needs to be changed to cd "/script"
	std::string cmd = "matlab -nodesktop -r \"plot_map('" + std::string(DATA_PATH) + filename + "', '" + labels[0] + "', '" + labels[1] + "', '" + labels[2] + "')\"";
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

// Convert SFML SoundBuffer to Vector
std::vector<short> toolbox::convertSFBuffer(sf::SoundBuffer& buffer)
{
	const short* data = &buffer.getSamples()[0];
	const int size = buffer.getSampleCount();

	std::vector<short> samples(data, data + size);
	return samples;
}

// Playback some DTMF tone sequence for 50ms each; sequence can be passed as argument (e.g. "1234")
void toolbox::playbackSequence(std::string args)
{
	// define default sequence
	//std::vector<int> sequence	= { 3, 7, 11, 15 };
	std::vector<int>   sequence = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	int duration				= DURATION;
	int pause					= PAUSE;

	// check arguments
	if (args != "")
	{
		// parse arguments
		std::istringstream			iss(args);
		std::vector<std::string>	splitArgs((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

		// reset sequence
		sequence.clear();

		// check sequence length to be even
		if (splitArgs[0].size() % 2 != 0)
		{
			std::cout << "Invalid sequence length; must be an even number.\n";
			return;
		}
		
		// extract sequence from args
		for (int i = 0; i < splitArgs[0].size(); i += 2)
		{
			std::string strnum = splitArgs[0].substr(i, 2);
			sequence.push_back(std::stoi(strnum));
		}

		// extra duration and pause from args
		if (splitArgs.size() == 3)
		{
			duration	= std::stoi(splitArgs[1]);
			pause		= std::stoi(splitArgs[2]);
		}
	}	
	
	// playback
	generator::playbackSequence(sequence, duration, pause);
}

///  Log Methods //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize decoder and log the payload (toneId) of decoded DTMF tones (allow self-messaging)
void toolbox::logDecoder()
{
	decoder::run(&dtmf::toolbox::printPayload, true);
}

// Initialize decoder and log the payload (toneId) of decoded DTMF tones (disallow self-messaging)
void toolbox::logDecoderQuiet()
{
	decoder::run(&dtmf::toolbox::printPayload, false);
}

// ...
void toolbox::logSequence()
{
	// sequence definition
	std::vector<int> sequence = { 3, 7, 11, 15 };

	// lambda definition
	auto lambda = [](int toneId)
	{
		return;
	};
	
	// start decoder
	decoder::run(lambda, true);

	// playback sequence
	generator::playbackSequence(sequence);

	// cleanup
	decoder::end();
}

// ...
void toolbox::logGoertzel(std::string args)
{
	using namespace std::chrono;

	// parse arguments
	std::istringstream			iss(args);
	std::vector<std::string>	splitArgs((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	if (splitArgs.size() < 4)
	{
		std::cout << "Invalid arguments. Use \"glog toneId(int) windowSize(int) thresholdSize(int) hanning(bool)\"; e.g. \"glog 0 5 500 1\".\n";
		return;
	}

	// determine test values from arguments
	const int		testToneId = std::stoi(splitArgs[0]);
	const int		testWindows = std::stoi(splitArgs[1]);
	const int		testThreshold = std::stoi(splitArgs[2]);
	const bool		useHanning = std::stoi(splitArgs[3]);
	const int		testFreqL = freq[testToneId / 4];
	const int		testFreqH = freq[(testToneId % 4) + 4];

	// variables
	sampler2							sampler([](std::vector<short> samples) {});
	std::vector<std::vector<short>>		queue(testWindows);
	std::vector<short>					buffer;
	int									counter = 0;

	high_resolution_clock				clock;
	time_point<high_resolution_clock>	timeStart;

	// print information
	std::cout << "Goertzel Logging:\n";
	std::cout << "TARGET FREQUENCIES:\t" << testFreqL << " Hz & " << testFreqH << " Hz\n";
	std::cout << "WINDOW SIZE:\t\t" << testWindows << "\n";
	std::cout << "MIN THRESHOLD:\t\t" << testThreshold << "\n";
	std::cout << "HANNING:\t\t" << (useHanning ? "true" : "false") << "\n";
	std::cout << "\nPress ESC to stop.\n\n";

	// prepare sampler
	sampler.prepare();

	while (true)
	{
		// break if ESC
		if (GetAsyncKeyState(VK_ESCAPE)) { break; }

		// update timer
		if (counter == 0)
		{
			timeStart = clock.now();
		}

		// get a sample
		queue[counter++] = sampler.sample();

		// wait until queue is full
		if (counter < testWindows) { continue; }

		// clear buffer
		buffer.clear();

		// combine sample chunks
		for (const auto& sampleChunks : queue)
		{
			buffer.insert(buffer.end(), sampleChunks.begin(), sampleChunks.end());
		}

		// reset counter (clear queue)
		counter = 0;

		// apply hanning
		if (useHanning)
		{
			processor::hanningWindow(buffer);
		}

		// perform goertzel
		auto magnitudeL = processor::goertzel(buffer, testFreqL);
		auto magnitudeH = processor::goertzel(buffer, testFreqH);

		// check threshold
		if (!(magnitudeL > testThreshold) || !(magnitudeH > testThreshold)) { continue; }

		// data
		std::cout << testFreqL << " Hz: " << magnitudeL << " | " << testFreqH << " Hz: " << magnitudeH
			<< " | " << static_cast<duration<double, std::milli>>(clock.now() - timeStart).count() << "ms" << std::endl;
	}

	// stop
	std::cout << "\nGoertzel Log stopped.\n\n";
}

// ...
void toolbox::logGoertzelAverage(std::string args)
{
	using namespace std::chrono;

	// parse arguments
	std::istringstream			iss(args);
	std::vector<std::string>	splitArgs((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	if (splitArgs.size() < 4)
	{
		std::cout << "Invalid arguments. Use \"glogat toneId(int) windowSize(int) thresholdSize(int) hanning(bool)\"; e.g. \"glog 0 5 500 1\".\n";
		return;
	}

	// determine test values from arguments
	const int		testToneId			= std::stoi(splitArgs[0]);
	const int		testWindows			= std::stoi(splitArgs[1]);
	const int		testThreshold		= std::stoi(splitArgs[2]);
	const bool		useHanning			= std::stoi(splitArgs[3]);
	const int		testFreqL			= freq[testToneId / 4];
	const int		testFreqH			= freq[(testToneId % 4) + 4];

	// variables
	sampler2							sampler([](std::vector<short> samples) {});
	std::deque<std::vector<short>>		queue(testWindows);
	std::vector<short>					buffer;

	high_resolution_clock				clock;
	time_point<high_resolution_clock>	timeStart;

	// print information
	std::cout << "Average Goertzel Logging:\n";
	std::cout << "TARGET FREQUENCIES:\t" << testFreqL << " Hz & " << testFreqH << " Hz\n";
	std::cout << "WINDOW SIZE:\t\t" << testWindows << "\n";
	std::cout << "MIN THRESHOLD:\t\t" << testThreshold << "\n";
	std::cout << "HANNING:\t\t" << (useHanning ? "true" : "false") << "\n";
	std::cout << "\nPress ESC to stop.\n\n";

	// prepare sampler
	sampler.prepare();

	while (true)
	{
		// break if ESC
		if (GetAsyncKeyState(VK_ESCAPE)) { break; }

		// get a sample
		queue.push_back(sampler.sample());

		// wait until queue is full
		if (queue.size() < testWindows) { continue; }

		// clear buffer
		buffer.clear();

		// combine sample chunks
		for (const auto& sampleChunks : queue)
		{
			buffer.insert(buffer.end(), sampleChunks.begin(), sampleChunks.end());
		}

		// remove first element of queue
		queue.pop_front();

		// apply hanning
		if (useHanning)
		{
			processor::hanningWindow(buffer);
		}

		// perform goertzel
		auto magnitudeL = processor::goertzel(buffer, testFreqL);
		auto magnitudeH = processor::goertzel(buffer, testFreqH);

		// check threshold
		if (!(magnitudeL > testThreshold) || !(magnitudeH > testThreshold)) { continue; }

		// data
		std::cout << testFreqL << " Hz:\t\t" << magnitudeL << " | " << testFreqH << " Hz:\t\t" << magnitudeH << std::endl;

	}

	// stop
	std::cout << "\nAverage Goertzel Log stopped.\n\n";
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

// Test sampler2 class; record a single chunk + 100ms chunks using callback
void toolbox::testSampler()
{
	sampler2* test = new sampler2([](std::vector<short> samples) {std::cout << "Got chunk [" << samples.size() << "]\n"; });
	test->prepare();

	auto chunk = test->sample();

	test->start();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	test->stop();

	delete test;
}

// Initialize decoder and execute keypress according to the payload (toneId) of percieved DTMF tones
void toolbox::testDecoderKeyboardReciever()
{
	decoder::run(&dtmf::toolbox::executePayload);
}

// ...
void toolbox::testDecoderKeyboardSender()
{
	generator::playback(0, 50);
}

// ...
void toolbox::testStepWindow(std::string args)
{
	using namespace std::chrono;

	// constants
	const int							latency = LATENCY;
	long long							delay = 0;
	const int							windowSize = SAMPLE_INTERVAL;
	const int							desiredWindows = 0 + 2 * (DURATION / SAMPLE_INTERVAL);
	const int							latencyWindows = STEP_WINDOW_SIZE + 1 * (latency / SAMPLE_INTERVAL);
	const bool							useHanning = true;

	const int							testToneId = 0;
	const int							testFreqL = freq[testToneId / 4];

	// variables
	sampler2							sampler([](std::vector<short> samples) {});
	std::thread							player;
	std::function<void()>				delayedPlayer;

	std::vector<short>					samples;
	std::map<double, short>				goertzelSingle;
	std::map<double, short>				goertzelAverage;
	std::map<double, std::vector<short>>chunks;

	high_resolution_clock				clock;
	time_point<high_resolution_clock>	timeStart;
	std::atomic<long long>				timeElapsed = 0;	// ms

	unsigned int						counter = 0;

	// prepare delayed playback thread
	delayedPlayer = [&]()
	{
		while (true)
		{
			timeElapsed = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

			if (timeElapsed > delay)
			{
				generator::playback(testToneId, DURATION);
				break;
			}
		}
	};

	// parse delay from arguments
	if (args != "")
	{
		delay = std::stoi(args);
	}

	// print information
	std::cout << "Step Window Test:\n";
	std::cout << "TARGET FREQUENCIES:\t"	<< testFreqL						<< " Hz\n";
	std::cout << "TARGET WINDOWS:\t\t"		<< desiredWindows					<< "\n";
	std::cout << "LATENCY WINDOWS:\t"		<< latencyWindows					<< "\n";
	std::cout << "DELAY:\t\t\t"				<< delay							<< " ms \n";
	std::cout << "SAMPLING RATE:\t\t"		<< SAMPLE_RATE						<< " Hz \n";
	std::cout << "SAMPLING INTERVAL:\t"		<< SAMPLE_INTERVAL					<< " ms \n";
	std::cout << "STEP WINDOW SIZE:\t"		<< STEP_WINDOW_SIZE					<< "\n";
	std::cout << "HANNING:\t\t"				<< (useHanning ? "true" : "false")	<< "\n\n";

	// goodnight
	std::this_thread::sleep_for(milliseconds(500));

	// prepare sampler
	sampler.prepare();
	std::vector<short>	samplesChunk(NUMPTS);

	// start clock
	std::cout << "Starting sampling ...\n";
	timeStart = clock.now();

	// start delayed playback thread
	player = std::thread(delayedPlayer);

	// datalogging loop
	for (int i = 0; i < (desiredWindows + latencyWindows); i++)
	{
		timeElapsed = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();
		chunks[timeElapsed] = sampler.sample();
	}

	// print info
	std::cout << "Finished sampling [" << static_cast<duration<double, std::milli>>(clock.now() - timeStart).count() << "ms]\n";

	// perform single goertzel
	for (auto pair : chunks)
	{
		auto samplesChunk = pair.second;

		samples.insert(samples.end(), samplesChunk.begin(), samplesChunk.end());

		if (useHanning)
		{
			processor::hanningWindow(samplesChunk);
		}

		goertzelSingle[pair.first] = processor::goertzel(samplesChunk, testFreqL);
	}

	// print info
	std::cout << "Finished processing single [" << static_cast<duration<double, std::milli>>(clock.now() - timeStart).count() << "ms]\n";

	// buffer & queue
	std::deque<std::vector<short>>		queue;
	std::vector<short>					buffer;

	// perform average goertzel
	for (auto pair : chunks)
	{
		// get chunk
		auto samplesChunk = pair.second;

		// fill samples vector
		samples.insert(samples.end(), samplesChunk.begin(), samplesChunk.end());

		// apply hanning
		if (useHanning)
		{
			processor::hanningWindow(samplesChunk);
		}

		// add to queue
		queue.push_back(samplesChunk);

		// check queue size
		if (queue.size() < STEP_WINDOW_SIZE) { continue; }

		// combine sample chunks into buffer
		for (const auto& chunk : queue)
		{
			buffer.insert(buffer.end(), chunk.begin(), chunk.end());
		}

		// pop front of queue
		queue.pop_front();

		// perform goertzel on buffer at "current time"
		goertzelAverage[pair.first] = processor::goertzel(buffer, testFreqL);
	}

	// print info
	std::cout << "Finished processing average [" << static_cast<duration<double, std::milli>>(clock.now() - timeStart).count() << "ms]\n";

	// cleanup
	player.join();

	// data here
	//toolbox::exportAudio(samples);
	//toolbox::exportSamples(samples);
	toolbox::plotMap(goertzelSingle, "step_single.dat", { "Single Step Goertzel Response", "Time [ms]", "Magnitude" });
	toolbox::plotMap(goertzelAverage, "step_average.dat", { "Average Step Goertzel Response", "Time [ms]", "Magnitude" });
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

	// stop recorder
	recorder.stop();

	// data here
	auto logFinal = toolbox::convertLatencyMap(logChunks, begin);
	toolbox::exportAudio(samples);
	toolbox::plotSamples(samples, "latency_samples.dat", { "Samples Plot", "Sample [N]", "Amplitude [dB]" });
	toolbox::plotMap(logFinal, "latency_map.dat", { "Map Plot", "Time [ms]", "Amplitude [dB]" });
}

// ..
double toolbox::testLatency2()
{
	using namespace std::chrono;

	// constants
	const int desiredChunks				= (SAMPLE_INTERVAL/10) + (DURATION/SAMPLE_INTERVAL) + (LATENCY / SAMPLE_INTERVAL);
	const int delayChunks				= 0;
	const int totalChunks				= desiredChunks + delayChunks;

	// variables
	high_resolution_clock				clock;
	time_point<high_resolution_clock>	timeStart;
	double								timeElapsed = 0;		// ms

	sampler2							sampler([](std::vector<short> samples) {});
	
	std::map<double, std::vector<short>>log;

	bool								hasPlayed = false;
	double								beginTime;

	// print information
	std::cout << "Latency Test:\n\n";

	// prepare sampler
	sampler.prepare();

	// start timer
	timeStart = clock.now();

	// sample loop
	for (int counter = 0; counter < totalChunks; counter++)
	{
		// play sound
		if (counter > delayChunks && !hasPlayed)
		{
			hasPlayed = true;
			beginTime = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();
			generator::playback(0, DURATION, true, false);
			log[beginTime] = { (int)AMPLITUDE_MAX };
		}
		
		// get samples chunk
		auto chunk = sampler.sample();

		// log chunk
		log[static_cast<duration<double, std::milli>>(clock.now() - timeStart).count()] = chunk;
	}

	// process data
	std::vector<double>			time;
	std::vector<short>			samples;
	std::map<double, short>		output;
	double						lastTime = beginTime;

	// split logged data map into time & samples
	for (auto const& x : log)
	{
		// vector of all samples
		for (int i = 0; i < x.second.size(); i++)
		{
			samples.push_back(x.second[i]);
		}

		// update deltaTime
		double deltatime = (x.first - lastTime) / double(x.second.size());

		// vector of all time
		for (int i = 0; i < x.second.size(); i++)
		{
			time.push_back(lastTime);
			lastTime += deltatime;
		}
	}

	// create output map
	for (int i = 0; i < samples.size(); i++)
	{
		output[time[i]] = samples[i];
	}

	double latencyBegin		= 0;
	double latencyEnd		= 0;
	bool  toneStart = false;

	for (auto const& x : output)
	{
		if (abs(x.second) > 5000 && toneStart)
		{
			latencyEnd = x.first;
			break;
		}

		if (x.second == AMPLITUDE_MAX && !toneStart)
		{
			latencyBegin= x.first;
			toneStart = true;
		}
	}

	double latency = latencyEnd - latencyBegin;

	// data here
	//toolbox::exportSamples(samples);
	//toolbox::plotMap(output, "latency_map.dat", { "Map Plot", "Time [ms]", "Amplitude [dB]" });

	// log result

	return latency;
	
}

// ...


void toolbox::averageLatency() 
{

	double sum = 0;

	for (int i = 0; i < 10; i++)
	{
		double value = toolbox::testLatency2();

		sum += value;
	}

	std::cout << "Average latency: " << sum / 10 << std::endl;
}


void toolbox::testGoertzel(std::string args)
{
	using namespace std::chrono;
	
	// parse arguments
	std::istringstream			iss(args);
	std::vector<std::string>	splitArgs((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	if (splitArgs.size() < 2)
	{
		std::cout << "Invalid arguments. Use \"gor2 duration(int) thresholdSize(int)\"; e.g. \"glog 50 500\".\n";
		return;
	}

	// determine test values from arguments
	const int					testDuration	= std::stoi(splitArgs[0]);
	const int					testThreshold	= std::stoi(splitArgs[1]);
	const bool					useHanning		= true;
	const std::vector<int>		testSequence	= { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	// print information
	std::cout << "Goertzel DTMF Test:\n";
	std::cout << "DURATION:\t\t" << testDuration << "\n";
	std::cout << "THRESHOLD:\t\t" << testThreshold << "\n";
	std::cout << "HANNING:\t\t" << (useHanning ? "true" : "false") << "\n";

	// variables
	sampler2									sampler([](std::vector<short> samples) {});

	std::vector<short>							samples;

	high_resolution_clock						clock;
	time_point<high_resolution_clock>			timeStart;
	double										timeElapsed = 0.f;	// ms

	// test loop
	for (const auto& testToneId : testSequence)
	{
		// safety sleep
		std::this_thread::sleep_for(milliseconds(500));
		
		// set test frequencies
		int	testFreqL	= freq[testToneId / 4];
		int	testFreqH	= freq[(testToneId % 4) + 4];

		// timing
		timeElapsed		= 0;
		timeStart		= clock.now();

		// play sound (no thread blocking)
		generator::playback(testToneId, testDuration, true);

		// sample and analyze
		while ((timeElapsed < ((testDuration*2) + LATENCY)))
		{
			// get sample chunk
			auto chunk = sampler.sample();
			samples.insert(samples.end(), chunk.begin(), chunk.end());

			// update timer
			timeElapsed = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();

			// apply hanning window
			processor::hanningWindow(chunk);

			// perform goertzel
			auto magnitudeL = processor::goertzel(chunk, testFreqL);
			auto magnitudeH = processor::goertzel(chunk, testFreqH);

			// check threshold
			if (!((magnitudeL > testThreshold) && (magnitudeH > testThreshold)))
			{
				continue;
			}

			// data
			std::cout << "ID: " << testToneId << " | " << testFreqL << " Hz: " << magnitudeL << " | " << testFreqH << " Hz: " << magnitudeH << std::endl;
		}

		// finished for a toneId
		std::cout << "Finished processing [" << timeElapsed << "ms]\n";
	}

	// data
	//toolbox::exportAudio(samples);
	//toolbox::plotSamples(samples);
}

void toolbox::testGoertzelVsFFT(){

	using namespace std::chrono;

	high_resolution_clock				clock;
	time_point<high_resolution_clock>	timeStart;

	auto buffer = generator::generateDTMF(0,10);
	auto samples = toolbox::convertSFBuffer(*buffer);
	auto samples2 = samples;

	timeStart = clock.now();
	processor::fft(samples);
	std::cout <<"fft-duration: " << static_cast<duration<double, std::milli>>(clock.now() - timeStart).count() <<" ms\n";


	timeStart = clock.now();
	samples2.resize(512, 0);
	processor::fft2(samples2);
	std::cout << "fft2-duration: " << static_cast<duration<double, std::milli>>(clock.now() - timeStart).count() << " ms\n";

	timeStart = clock.now();
	processor::goertzelArray(samples);
	std::cout << "Gor-duration: " << static_cast<duration<double, std::milli>>(clock.now() - timeStart).count() << " ms\n";
};

// Test Goertzel on generated samples of a specified toneId and duration
void toolbox::testGeneratorGoertzel(std::string args)
{
	// parse arguments
	std::istringstream			iss(args);
	std::vector<std::string>	splitArgs((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	if (splitArgs.size() < 2)
	{
		std::cout << "Invalid arguments. Use \"gor toneId(int) duration(int)\"; e.g. \"gor 0 50\".\n";
		return;
	}

	// determine test values from arguments
	const int		testToneId		= std::stoi(splitArgs[0]);
	const int		testDuration	= std::stoi(splitArgs[1]);
	const int		testFreqL		= freq[testToneId / 4];
	const int		testFreqH		= freq[(testToneId % 4) + 4];

	// print information
	std::cout << "Generator Goertzel Test:\n";
	std::cout << "TARGET TONEID:\t"			<< testToneId							<< "\n";
	std::cout << "TARGET FREQUENCIES:\t"	<< testFreqL << " Hz & " << testFreqH	<< " Hz\n";
	std::cout << "DURATION:\t\t"			<< testDuration							<< "ms\n";

	// generate buffer
	auto buffer = generator::generateDTMF(testToneId, testDuration);
	auto samples1 = toolbox::convertSFBuffer(*buffer);
	auto samples2 = samples1;

	// apply hanning window
	processor::hanningWindow(samples2);

	// perform tests
	auto magnitude1 = processor::goertzel(samples1, testFreqL);
	auto magnitude2 = processor::goertzel(samples2, testFreqH);

	// export
	toolbox::plotSamples(samples1, "s1.dat", { "Samples1 Plot [-HW][" + std::to_string(testFreqL) + "Hz]", "Sample [N]", "Amplitude [dB]" });
	toolbox::plotSamples(samples2, "s2.dat", { "Samples2 Plot [+HW][" + std::to_string(testFreqL) + "Hz]", "Sample [N]", "Amplitude [dB]" });
	//toolbox::exportAudio(samples1, "s1.wav");

	// log
	std::cout << "\nGoertzel Magnitude Response:\n"
		<< "S1 -HW:\t\t" << magnitude1 << "\n"
		<< "S2 +HW:\t\t" << magnitude2 << "\n"
		<< "\n";
}

}