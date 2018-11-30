#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <vector>
#include <queue>
#include <array>
#include <stdlib.h>
#include <functional>
#include <windows.h>

#include "decoder.h"
#include "sampler.h"
#include "sampler2.h"
#include "generator.h"
#include "processor.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace decoder
{
	using namespace std::chrono;

	// Private Members
	std::atomic<state>					status = state::unitialized;
	std::deque<std::vector<short>>		queue;
	std::vector<short>					buffer;
	std::mutex							queueMutex;
	std::condition_variable				queueCondition;
	std::thread							worker;
	sampler*							rec1;
	sampler2*							rec2;

	bool								allowPlayback = false;
	std::atomic<bool>					running;
	
	std::array<float, 8>				previousGoertzelArray;
	bool								previousThresholdBroken;
	int									previousToneId = -1;

	high_resolution_clock				clock;
	time_point<high_resolution_clock>	debounce;
	
	std::function<void(uint)>			callback;

	// Private Methods
	void								threadBuffered();
	void								threadInstant();
	void								threadContinuous();

	void								decode(std::vector<short> &samples);
	void								decode2(std::vector<short> &samples);
	void								decode3(std::vector<short> &samples);

	void								appendQueue(std::vector<short> samples);
	
	bool								thresholdTest(std::array<float, 8> goertzelArray);
	std::array<int, 2>					extractIndexes(std::array<float, 8> &goertzelArray);
	int									extractToneId(std::array<int, 2> &indexes);
}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize & run the decoder
void decoder::run(std::function<void(uint toneId)> callback, bool allowPlayback)
{
	// initialize variables
	decoder::callback		= callback;
	//decoder::rec1			= new sampler(&decoder::appendQueue, allowPlayback);
	decoder::rec2			= new sampler2(&decoder::appendQueue, allowPlayback);
	decoder::allowPlayback	= allowPlayback;

	// start sampler
	//decoder::rec1->start(SAMPLE_RATE);
	decoder::rec2->start(); // sampler 2
	decoder::status			= state::running;
	
	// start worker thread
	decoder::debounce		= decoder::clock.now();
	decoder::running		= true;
	decoder::worker			= std::thread(&decoder::threadContinuous);
}	

// End the decoder
void decoder::end()
{
	// stop and delete sampler
	decoder::rec2->stop();
	delete decoder::rec2;

	// stop worker thread
	decoder::status		= state::unitialized;
	decoder::running	= false;
	decoder::worker.join();
}

// Thread function (step buffered decode)
void decoder::threadBuffered()
{
	while (decoder::running)
	{		
		// check status
		if (decoder::status != state::running)
		{
			continue;
		}

		// queue critical section
		decoder::queueMutex.lock();

			// check that queue has enough elements
			if (decoder::queue.size() < STEP_WINDOW_SIZE)
			{
				decoder::queueMutex.unlock();
				continue;
			}
			
			// clear buffer
			decoder::buffer.clear();

			// compose buffer from queue
			for (const auto& sampleChunks : decoder::queue)
			{  
				decoder::buffer.insert(decoder::buffer.end(), sampleChunks.begin(), sampleChunks.end());
			}

			// pop first element of queue
			decoder::queue.pop_front();

		decoder::queueMutex.unlock();
		
		// decode the copied samples
		decoder::decode(buffer);
	}
}

// Thread function (instant decode)
void decoder::threadInstant()
{
	while (decoder::running)
	{
		// check status
		if (decoder::status != state::running)
		{
			continue;
		}

		// queue critical section
		decoder::queueMutex.lock();

			// check queue size
			if (decoder::queue.empty())
			{
				decoder::queueMutex.unlock();
				continue;
			}

			// make copy of first element in queue
			auto samplesCopy = decoder::queue.front();

			// pop the element
			decoder::queue.pop_front();

		decoder::queueMutex.unlock();

		// decode the copied samples
		decoder::decode(samplesCopy);
	}
}

// ...
void decoder::threadContinuous()
{
	while (decoder::running)
	{
		/*
		The method is protected by a mutex, which is locked using scope guard std::unique_lock. If the queue is empty,
		we wait on the condition variable cond_. This releases the lock to other threads and blocks until we are
		notified that the condition has been met.

		https://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/
		*/

		// aqcuire queue mutex
		std::unique_lock<std::mutex> queueLock(decoder::queueMutex);

		// check if queue empty; wait for conditional variable if false
		while (queue.empty())
		{
			decoder::queueCondition.wait(queueLock);
		}

		// make copy of first element in queue
		auto samplesCopy = decoder::queue.front();

		// pop the element
		decoder::queue.pop_front();

		// unclock queue
		queueLock.unlock();

		// decode the copied samples
		decoder::decode3(samplesCopy);
	}
}

// Add a (copy of) vector of samples to decoding queue
void decoder::appendQueue(std::vector<short> samples)
{	
	decoder::queueMutex.lock();

	//std::cout << "[QUEUE] Adding to queue with [" << decoder::queue.size() << "] elements.\n";
	decoder::queue.push_back(samples);

	decoder::queueMutex.unlock();

	decoder::queueCondition.notify_one();
}

// Test whether two of magnitudes surpass their according thresholds; return boolean
bool decoder::thresholdTest(std::array<float, 8> goertzelArray)
{
	int counter = 0;

	for (int i = 0; i < 8; i++)
	{
		if (goertzelArray[i] > freqThresholds[i])
		{
			counter++;
		}
	}

	return (counter == 2);
}

// Convert goertzelArray to indexes (row & column) of two most prominent frequencies; return array[2]
std::array<int, 2> decoder::extractIndexes(std::array<float, 8> &goertzelArray)
{
	float magnitudeLowMax		= 0;
	float magnitudeHighMax		= 0;

	std::array<int, 2> indexes	= { -1, -1 };

	// Iterate through array of goertzel magnitudes
	for (uint i = 0; i < goertzelArray.size(); i++)
	{
		// define magnitude & threshold for current frequency index (i)
		auto magnitude = goertzelArray[i] * freqMultiplier[i];
		auto threshold = freqThresholds[i] * TH_MULTIPLIER;

		// low frequencies
		if (i < 4 && magnitude > threshold && magnitude > magnitudeLowMax)
		{
			magnitudeLowMax = magnitude;
			indexes[0] = i;
		}

		// high frequencies
		else if (i >= 4 && magnitude > threshold && magnitude > magnitudeHighMax)
		{
			magnitudeHighMax = magnitude;
			indexes[1] = (i - 4);
		}
	}

	// return array of indexes
	return indexes;
}

// Convert DTMF indexes (row & column) to toneID (0-15); return int
int decoder::extractToneId(std::array<int, 2> &indexes)
{
	int indexLow	= indexes[0];
	int indexHigh	= indexes[1];

	if ((indexLow * indexHigh) < 0)
	{
		return -1;
	}

	return (indexLow * 4 + indexHigh);
}

// Decode an chunk of samples from the queue
void decoder::decode(std::vector<short> &samples)
{	
	decoder::status = state::working;
	
	// decode
	//std::cout << "[DECODER] Decoding [" << samples.size() << "] samples...\n\n";

	// check generator overlapping
	if (!decoder::allowPlayback)
	{
		// define time since last generator playback
		auto timeSincePlayback = (int)static_cast<duration<double, std::milli>>(decoder::clock.now() - generator::getTimestamp()).count();
		
		// return if overlapping
		// VALUES NEED TO BE CHANGED WHEN NOT LOOKING AT AVERAGE GOERTZEL
		if (timeSincePlayback > (LATENCY - DURATION * 2) && timeSincePlayback < (LATENCY + LATENCY_BUFFER + DURATION * 2))
		{
			//std::cout << "OVERLAP! [" << timeSincePlayback << "ms]" << std::endl;
			decoder::status = state::running;
			return;
		}
	}
	
	// check debounce
	if ((int)static_cast<duration<double, std::milli>>(decoder::clock.now() - decoder::debounce).count() < DEBOUNCE)
	{
		decoder::status = state::running;
		return;
	}

	// apply hanning window to samples
	processor::hanningWindow(samples);

	// compile goertzelArray for all DTMF frequencies
	auto goertzelArray = processor::goertzelArray(samples);

	// extract indexes (row & column) of most prominent frequencies
	auto indexes = decoder::extractIndexes(goertzelArray);

	// convert indexes to DTMF toneID
	auto toneId = decoder::extractToneId(indexes);

	// callback
	if (toneId >= 0)
	{
		decoder::debounce = decoder::clock.now();
		decoder::callback(toneId);
	}
	
	decoder::status = state::running;
}

// Decode an chunk of samples from the queue (version 2)
void decoder::decode2(std::vector<short> &samples)
{
	decoder::status = state::working;

	// decode
	//std::cout << "[DECODER] Decoding [" << samples.size() << "] samples...\n\n";

	// check debounce
	if ((int)static_cast<duration<double, std::milli>>(decoder::clock.now() - decoder::debounce).count() < DEBOUNCE)
	{
		decoder::status = state::running;
		return;
	}

	// apply hanning window to samples
	processor::hanningWindow(samples);

	// compile goertzelArray for all DTMF frequencies
	auto goertzelArray = processor::goertzelArray(samples);

	// check if any values surpass thresholds
	bool thresholdBroken = decoder::thresholdTest(goertzelArray);

	/*

	A surpassed boolean is needed for every DTMF frequency!	Also update
	thresholdTest so only DTMF tones pass through (>4).
	
	*/

	// check if previous treshold was surpassed and current was not
	if (previousThresholdBroken && !thresholdBroken)
	{
		// extract indexes (row & column) of most prominent frequencies of previousGoertzel
		auto indexes = decoder::extractIndexes(previousGoertzelArray);

		// convert indexes to DTMF toneID
		auto toneId = decoder::extractToneId(indexes);

		// callback
		if (toneId >= 0)
		{
			decoder::debounce = decoder::clock.now();
			decoder::callback(toneId);
		}
	}

	// save values for next iteration
	previousGoertzelArray		= goertzelArray;
	previousThresholdBroken		= thresholdBroken;

	decoder::status = state::running;
}

// ...
void decoder::decode3(std::vector<short> &samples)
{
	// check debounce
	if (static_cast<duration<double, std::milli>>(decoder::clock.now() - decoder::debounce).count() < DEBOUNCE2)
	{
		return;
	}

	// check generator overlap
	if (!decoder::allowPlayback)
	{
		// calculate time since last generator playback
		auto timeSincePlayback = static_cast<duration<double, std::milli>>(decoder::clock.now() - generator::getTimestamp()).count();

		// return if overlapping
		if (timeSincePlayback < (LATENCY + DURATION + LATENCY_BUFFER))
		{
			return;
		}
	}

	// log
	//std::cout << "[DECODER] Decoding [" << samples.size() << "] samples...\n\n";

	// update status
	decoder::status = state::working;

	// apply hanning window
	//processor::hanningWindow(samples);

	// compile goertzelArray for all DTMF frequencies
	auto goertzelArray = processor::goertzelArray(samples);

	// extract indexes (row & column) of most prominent frequencies
	auto indexes = decoder::extractIndexes(goertzelArray);

	// check harmonies
	;

	// convert indexes to DTMF toneId
	auto toneId = decoder::extractToneId(indexes);

	// callback
	if (toneId >= 0 && previousToneId == toneId)
	{
		processor::printGoertzelArray(goertzelArray);
		decoder::debounce = decoder::clock.now();
		decoder::callback(toneId);
		previousToneId = -1;
	}
	else
	{
		previousToneId = toneId;
	}

	// update status
	decoder::status = state::running;

}