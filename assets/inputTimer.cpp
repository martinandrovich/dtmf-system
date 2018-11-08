#include <iostream>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <vector>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <assert.h>

double shortestDur(std::vector<double> durations)
{
	double shortest;
	if (durations.size() > 0)
	{
		shortest = durations[0];
	}
	else if (durations.size() <= 0)
	{
		return NAN;
	}

	for (unsigned i = 0; i < durations.size(); i++)
	{
		if (durations[i] < shortest)
		{
			shortest = durations[i];
		}

	}

	return shortest;
}

double averageDur(std::vector<double> durations)
{
	double total = 0;

	for (unsigned i = 0; i < durations.size(); i++ )
	{
		total += durations[i];
	}
	if (durations.size() < 1)
	{
		return NAN;
	}
	return total / double(durations.size());
}

void pop_front(std::vector<double>& vec) // Used to remove the first element in inputDelta
{
	assert(!vec.empty());
	vec.front() = std::move(vec.back());
	vec.pop_back();
}

double sum(std::vector<double> durations)
{
	double sum = 0;
	for (unsigned i = 0; i < durations.size(); i++)
	{
		sum += durations[i];
	}

	return sum;
}


int main() {
	using namespace std::chrono;

	//-------------------------- Global times -----------------------------------------------------------------------
	high_resolution_clock::time_point	startTime;
	high_resolution_clock::time_point	endTime;
	duration<double>					totalTime;

	high_resolution_clock::time_point	inputTime;
	std::vector <double>                inputDelta;
	
	//---------------------------------------------------------------------------------------------------------------


	//-------------------------- Individual Key and duration --------------------------------------------------------


	sf::Keyboard::Key	Esc		= sf::Keyboard::Escape;

	sf::Keyboard::Key	Bomb	= sf::Keyboard::Space;
	std::vector<double>			durationsBomb;

	sf::Keyboard::Key	Up		= sf::Keyboard::Up;
	std::vector<double>			durationsUp;

	sf::Keyboard::Key	Down	= sf::Keyboard::Down;
	std::vector<double>			durationsDown;

	sf::Keyboard::Key	Right	= sf::Keyboard::Right;
	std::vector<double>			durationsRight;

	sf::Keyboard::Key	Left	= sf::Keyboard::Left;
	std::vector<double>			durationsLeft;

	//----------------------------------------------------------------------------------------------------------------------



	//--------------------------- Waiting on start condition ---------------------------------------------------------------
	std::cout << "Will start to monitor when game is started\n\n";

	while (!sf::Keyboard::isKeyPressed(Bomb) && !sf::Keyboard::isKeyPressed(Up) && !sf::Keyboard::isKeyPressed(Down) && !sf::Keyboard::isKeyPressed(Left) && !sf::Keyboard::isKeyPressed(Right))
	{
		//Wait for start condition
	}
	//----------------------------------------------------------------------------------------------------------------------



	system("cls"); //Clear Consol

	std::cout << "Press Esc to stop monitoring\n";

	startTime = high_resolution_clock::now(); //setting start time for monitoring


	//--------------------------- Start monitoring -----------------------------------------------------------------------

	while (!sf::Keyboard::isKeyPressed(Esc)) {

		

		if (sf::Keyboard::isKeyPressed(Bomb))
		{
			//Saving time since last input (if first input NaN)
			inputTime = high_resolution_clock::now();
			duration<double> dur = duration_cast<duration<double>>(inputTime- endTime);
			inputDelta.push_back(dur.count());

			//Starting time for start input;
			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			while (sf::Keyboard::isKeyPressed(Bomb))
			{

			}
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			//Saving time since input start
			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
			durationsBomb.push_back(time_span.count());

			endTime = high_resolution_clock::now(); //Seting time end after last input
		}
		else if(sf::Keyboard::isKeyPressed(Up))
		{
			inputTime = high_resolution_clock::now();
			duration<double> dur = duration_cast<duration<double>>(inputTime - endTime);
			inputDelta.push_back(dur.count());

			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			while (sf::Keyboard::isKeyPressed(Up))
			{

			}
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
			durationsUp.push_back(time_span.count());

			endTime = high_resolution_clock::now(); //Seting time end after last input
		}
		else if (sf::Keyboard::isKeyPressed(Down))
		{
			inputTime = high_resolution_clock::now();
			duration<double> dur = duration_cast<duration<double>>(inputTime - endTime);
			inputDelta.push_back(dur.count());

			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			while (sf::Keyboard::isKeyPressed(Down))
			{

			}
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
			durationsDown.push_back(time_span.count());

			endTime = high_resolution_clock::now(); //Seting time end after last input
		}
		else if (sf::Keyboard::isKeyPressed(Right))
		{
			inputTime = high_resolution_clock::now();
			duration<double> dur = duration_cast<duration<double>>(inputTime - endTime);
			inputDelta.push_back(dur.count());

			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			while (sf::Keyboard::isKeyPressed(Right))
			{

			}
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
			durationsRight.push_back(time_span.count());

			endTime = high_resolution_clock::now(); //Seting time end after last input
		}
		else if (sf::Keyboard::isKeyPressed(Left))
		{
			inputTime = high_resolution_clock::now();
			duration<double> dur = duration_cast<duration<double>>(inputTime - endTime);
			inputDelta.push_back(dur.count());

			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			while (sf::Keyboard::isKeyPressed(Left))
			{

			}
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
			durationsLeft.push_back(time_span.count());

			endTime = high_resolution_clock::now(); //Seting time end after last input
		}

	}
	
	

	//-------------------------------------- end of monitoring ---------------------------------------------------------------





	totalTime = duration_cast<duration<double>>(endTime - startTime);

	int totalCount = inputDelta.size(); //The number of inputs is equal to all inputs

	pop_front(inputDelta);				//but the first is removed because its not really a delta, there were no input before it.




	//-------------------------------------- output gathered information -----------------------------------------------------
	system("cls");

	std::cout << "Total time = " << totalTime.count() <<" s" << std::endl << std::endl;

	std::cout << std::setw(5) << std::left << "Key" << std::setw(2) << std::left << "|" << std::setw(7) << std::left << "Amount" << std::setw(2) << std::left << "|" << std::setw(22) << std::left << "Shortest duration" << std::setw(2) << std::left << "|" << std::setw(10) << std::left << "Average duration" << std::endl;
	std::cout << std::setw(5) << std::left << "Bomb" << std::setw(2) << std::left << "|" << std::setw(7) << std::left << durationsBomb.size() << std::setw(2) << std::left << "|" << std::setw(12) << std::left << shortestDur(durationsBomb) << std::setw(10) << std::left << "s" << std::setw(2) << std::left << "|" << std::setw(12) << std::left << averageDur(durationsBomb) << "s" << std::endl;
	std::cout << std::setw(5) << std::left << "Up" << std::setw(2) << std::left << "|" << std::setw(7) << std::left << durationsUp.size() << std::setw(2) << std::left << "|" << std::setw(12) << std::left << shortestDur(durationsUp) << std::setw(10) << std::left << "s" << std::setw(2) << std::left << "|" << std::setw(12) << std::left << averageDur(durationsUp) << "s" << std::endl;
	std::cout << std::setw(5) << std::left << "Down" << std::setw(2) << std::left << "|" << std::setw(7) << std::left << durationsDown.size() << std::setw(2) << std::left << "|" << std::setw(12) << std::left << shortestDur(durationsDown)<< std::setw(10) << std::left << "s" << std::setw(2) << std::left << "|" << std::setw(12) << std::left << averageDur(durationsDown) << "s" << std::endl;
	std::cout << std::setw(5) << std::left << "Right" << std::setw(2) << std::left << "|" << std::setw(7) << std::left << durationsRight.size() << std::setw(2) << std::left << "|" << std::setw(12) << std::left << shortestDur(durationsRight) << std::setw(10) << std::left << "s" << std::setw(2) << std::left << "|" << std::setw(12) << std::left << averageDur(durationsRight) << "s" << std::endl;
	std::cout << std::setw(5) << std::left << "Left" << std::setw(2) << std::left << "|" << std::setw(7) << std::left << durationsLeft.size() << std::setw(2) << std::left << "|" << std::setw(12) << std::left << shortestDur(durationsLeft) << std::setw(10) << std::left << "s" << std::setw(2) << std::left << "|" << std::setw(12) << std::left << averageDur(durationsLeft) << "s" << std::endl;
	std::cout << std::setw(5) << std::left << "" << std::setw(2) << std::left << "|" << std::setw(7) << std::left << "" << std::setw(2) << std::left << "|" << std::setw(22) << std::left << "" << std::setw(2) << std::left << "|" << std::setw(10) << std::left << "" << std::endl;
	std::cout << std::setw(5) << std::left << "" << std::setw(2) << std::left << "|" << std::setw(7) << std::left << "total" << std::setw(2) << std::left << "|" << std::setw(22) << std::left << "Shortest time between" << std::setw(2) << std::left << "|" << std::setw(10) << std::left << "Average time between" << std::endl;
	std::cout << std::setw(5) << std::left << "" << std::setw(2) << std::left << "|" << std::setw(7) << std::left << totalCount << std::setw(2) << std::left << "|" << std::setw(12) << std::left << shortestDur(inputDelta) << std::setw(10) << std::left << "s" << std::setw(2) << std::left << "|" << std::setw(12) << std::left << averageDur(inputDelta) << "s" << std::endl << std::endl;


	std::cout << "key-strokes pr.s : " << (1/totalTime.count())*totalCount << std::endl<<std::endl;

	double checkTime = sum(durationsBomb) + sum(durationsDown) + sum(durationsLeft) + sum(durationsRight) + sum(durationsUp) + sum(inputDelta);


	double pro = (abs(checkTime-totalTime.count())/((checkTime+totalTime.count())/2))*100;

	std::cout << "There are a discrepancy of " << pro << "%" << std::endl;
	//----------------------------------------------------------------------------------------------------------------------
	int wait;
	std::cin >> wait;
}