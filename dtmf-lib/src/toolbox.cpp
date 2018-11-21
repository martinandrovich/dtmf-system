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
	void logPayload(uint toneId);
	void executePayload(uint toneId);
	void pressKey(int key, int pause);
	
	void makeDataDirectory();
	std::vector<short>	convertSFBuffer(sf::SoundBuffer& buffer);
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
void toolbox::makeDataDirectory()
{
	//system(std::string("mkdir \"" + std::string(DATA_PATH) + "\"").c_str());
	CreateDirectory(DATA_PATH, NULL);
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

// ...
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

// ...
void toolbox::exportAudio(std::vector<short> &samples, std::string filename)
{
	// create data directory & update filename
	toolbox::makeDataDirectory();
	filename = DATA_PATH + filename;

	sf::SoundBuffer buffer;
	buffer.loadFromSamples(&samples[0], samples.size(), 1, SAMPLE_RATE);

	buffer.saveToFile(filename);
}

// ...
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

// ...
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

// Playback some DTMF tone sequence for 50ms each; sequence can be passed as argument (e.g. "1234")
void toolbox::playbackSequence(std::string args)
{
	// define default sequence
	std::vector<int> sequence	= { 3, 7, 11, 15 };
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

// Initialize decoder and log the payload (toneId) of decoded DTMF tones
void toolbox::logDecoder()
{
	decoder::run(&dtmf::toolbox::logPayload);
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

	// variables
	steady_clock				clock;
	time_point<steady_clock>	timeStart;
	long long					timeElapsed		= 0;		// ms

	int							frequency		= 697;
	int							delay			= 0;

	int							numToDiscard	= STEP_WINDOW_SIZE * 5;
	int							numToStore		= STEP_WINDOW_SIZE * 2;
	int							counter			= 0;

	std::vector<short>			samples;
	std::map<int, float>		goertzel;
	std::vector<short>			goertzel2(numToStore);
	sampler*					recorder;

	std::atomic<bool>			logging(false);

	// parse delay from arguments
	if (args != "")
	{
		delay = std::stoi(args);
	}

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
	//toolbox::exportAudio(samples);

	// clean up
	delete recorder;
}

// ...
void toolbox::testStepWindow2(std::string args)
{
	using namespace std::chrono;

	// constants
	const int							latency				= 100;	// ms
	long long							delay				= 0;
	const int							windowSize			= SAMPLE_INTERVAL;
	const int							desiredWindows		= 0 + 2 * (DURATION / SAMPLE_INTERVAL);
	const int							latencyWindows		= 1 + 1 * (latency / SAMPLE_INTERVAL);

	const int							testToneId			= 0;
	const int							testFreq			= freq[testToneId / 4];

	// variables
	sampler2							sampler([](std::vector<short> samples) {});
	std::thread							player;
	std::function<void()>				delayedPlayer;

	std::vector<short>					samples;
	std::map<double, short>				goertzel;
	std::map<double, std::vector<short>>chunks;

	high_resolution_clock				clock;
	time_point<high_resolution_clock>	timeStart;
	std::atomic<long long>				timeElapsed			= 0;	// ms

	unsigned int						counter				= 0;

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
	std::cout << "Step Window Analysis (v2): ["
		<< "freq: "			<< testFreq			<< " Hz | "
		<< "tone: "			<< DURATION			<< " ms | "
		<< "delay: "		<< delay			<< " ms | "
		<< "interval: "		<< SAMPLE_INTERVAL	<< " ms | "
		<< "sample rate: "	<< SAMPLE_RATE		<< " Hz "
	<< "]\n";

	// goodnight
	std::this_thread::sleep_for(milliseconds(500));	

	// prepare sampler
	sampler.prepare();
	std::vector<short>	samplesChunk(NUMPTS);

	// start clock
	std::cout << "Started sampling [0.00ms]\n";
	timeStart = clock.now();

	// start delayed playback thread
	player = std::thread(delayedPlayer);

	// datalogging loop
	for (int i = 0; i < (desiredWindows + latencyWindows); i++)
	{
		timeElapsed = static_cast<duration<double, std::milli>>(clock.now() - timeStart).count();
		chunks[timeElapsed] = sampler.sample();
	}

	std::cout << "Finished sampling [" << static_cast<duration<double, std::milli>>(clock.now() - timeStart).count() << "ms]\n";

	// perform goertzel
	for (auto pair : chunks)
	{
		auto samplesChunk = pair.second;

		samples.insert(samples.end(), samplesChunk.begin(), samplesChunk.end());

		processor::hanningWindow(samplesChunk);
		goertzel[pair.first] = processor::goertzel(samplesChunk, testFreq);
	}

	std::cout << "Finished processing [" << static_cast<duration<double, std::milli>>(clock.now() - timeStart).count() << "ms]\n";

	// cleanup
	player.join();

	// data here
	toolbox::exportAudio(samples);
	toolbox::exportSamples(samples);
	toolbox::plotMap(goertzel, "map_plot.dat", { "Goertzel Response", "Time [ms]", "Magnitude" });
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

// Test sampler2 class; record a single chunk using callback
void toolbox::testSampler2()
{
	sampler2* test = new sampler2([](std::vector<short> samples) {std::cout << "Got chunk [" << samples.size() << "]\n"; });
	test->prepare();

	auto chunk = test->sample();
	//toolbox::exportAudio(chunk);
	test->start();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	test->stop();
	delete test;
}

// ...
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
	const int		testFreq		= freq[testToneId / 4];
	const int		testDuration	= std::stoi(splitArgs[1]);

	// init log
	std::cout << "Running Single Goertzel Test [" << testToneId << " | " << testFreq << "Hz" << " | " << testDuration << "ms" << "] ...\n\n";

	// generate buffer
	auto buffer		= generator::generateDTMF(testToneId, testDuration);
	auto samples1	= toolbox::convertSFBuffer(*buffer);
	auto samples2	= samples1;

	// perform tests
	processor::hanningWindow(samples2);

	auto magnitude1	= processor::goertzel(samples1, testFreq);
	auto magnitude2 = processor::goertzel(samples2, testFreq);

	// export
	toolbox::plotSamples(samples1, "s1.dat", { "Samples1 Plot [-HW]", "Sample [N]", "Amplitude [dB]" });
	toolbox::plotSamples(samples2, "s2.dat", { "Samples2 Plot [+HW]", "Sample [N]", "Amplitude [dB]" });
	toolbox::exportAudio(samples1, "s1.wav");

	// log
	std::cout << "\nGoertzel Magnitude Response:\n"
		<< "S1 -HW:\t\t" << magnitude1 << "\n"
		<< "S2 +HW:\t\t" << magnitude2 << "\n"
		<< "\n";
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
	const int		testToneId		= std::stoi(splitArgs[0]);
	const int		testWindows		= std::stoi(splitArgs[1]);
	const int		testThreshold	= std::stoi(splitArgs[2]);
	const bool		useHanning		= std::stoi(splitArgs[3]);
	const int		testFreqL		= freq[testToneId / 4];
	const int		testFreqH		= freq[(testToneId % 4) + 4];	

	// variables
	sampler2							sampler([](std::vector<short> samples) {});
	std::vector<std::vector<short>>		queue(testWindows);
	std::vector<short>					buffer;
	int									counter = 0;

	high_resolution_clock				clock;
	time_point<high_resolution_clock>	timeStart;

	// print information
	std::cout << "Goertzel Logging:\n";
	std::cout << "TARGET FREQUENCIES:\t"		<< testFreqL << " Hz & " << testFreqH << " Hz\n";
	std::cout << "WINDOW SIZE:\t\t"				<< testWindows << "\n";
	std::cout << "MIN THRESHOLD:\t\t"			<< testThreshold << "\n";
	std::cout << "HANNING:\t\t"					<< (useHanning ? "true" : "false") << "\n";
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
		if (counter < testWindows)	{ continue;	}

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

// ...
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

}