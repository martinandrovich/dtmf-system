#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>

sf::SoundBuffer globalBuffer;

float goertzel_mag(int numSamples, int TARGET_FREQUENCY, int SAMPLING_RATE,const sf::Int16 *data)
{
	int     k, i;
	float   floatnumSamples;
	float   omega, sine, cosine, coeff, q0, q1, q2, magnitude, real, imag;

	float   scalingFactor = numSamples / 2.0;

	floatnumSamples = (float)numSamples;
	k = (int)(0.5 + ((floatnumSamples * TARGET_FREQUENCY) / SAMPLING_RATE));
	omega = (2.0 * 3.14159265359 * k) / floatnumSamples;
	sine = sin(omega);
	cosine = cos(omega);
	coeff = 2.0 * cosine;
	q0 = 0;
	q1 = 0;
	q2 = 0;

	for (i = 0; i < numSamples; i++)
	{
		q0 = coeff * q1 - q2 + data[i];
		q2 = q1;
		q1 = q0;
	}

	// calculate the real and imaginary results
	// scaling appropriately
	real = (q1 - q2 * cosine) / scalingFactor;
	imag = (q2 * sine) / scalingFactor;

	magnitude = sqrtf(real*real + imag * imag);
	return magnitude;
}


struct CRecorder : sf::SoundRecorder
{
	CRecorder() { }

	~CRecorder() { stop(); }

	bool onProcessSamples(const int16_t* samples, const size_t sampleCount)
	{

		return true;
	}

	bool onStart()
	{
		setProcessingInterval(sf::milliseconds(20));
		return true;
	}

	void onStop()
	{

	}
};


int main()
{
	while (true)
	{
		// Check that the device can capture audio
		if (sf::SoundRecorder::isAvailable() == false)
		{
			std::cout << "Sorry, audio capture is not supported by your system" << std::endl;
			return EXIT_SUCCESS;
		}

		std::cout << "Press enter to start recording audio";
		std::cin.ignore(10000, '\n');


		sf::SoundBufferRecorder recorder;
		std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();

		std::string inputDevice = availableDevices[0];


		// set the device
		if (!recorder.setDevice(inputDevice))
		{
			// error: device selection failed

		}

		recorder.start(44100);
		std::cout << "Recording... press enter to stop";
		std::cin.ignore(10000, '\n');
		recorder.stop();


		const sf::SoundBuffer buffer = recorder.getBuffer();


		std::cout << "Sound information:" << std::endl;
		std::cout << " " << buffer.getDuration().asSeconds() << " seconds" << std::endl;
		std::cout << " " << buffer.getSampleRate() << " samples / seconds" << std::endl;
		std::cout << " " << buffer.getChannelCount() << " channels" << std::endl;

		char choice;
		std::cout << "What do you want to do with captured sound (p = play, v = visualize, g=Goertzel) ? ";
		std::cin >> choice;
		std::cin.ignore(10000, '\n');

		sf::Sound sound(buffer);




		if (choice == 'g')    //goertzel af samples;
		{
			char choiceg;
			std::cout << "What do you want Goertzel (c = constant update, s = singel update)";
			std::cin >> choiceg;
			std::cin.ignore(10000, '\n');

			if(choiceg == 's')
			std::cout << goertzel_mag(buffer.getSampleCount(), 697, 44100, buffer.getSamples()) << "  697 hz  &" << goertzel_mag(buffer.getSampleCount(), 770, 44100, buffer.getSamples()) << "  770 hz  &" << std::endl;
			else
			{
				float test697;
				float test770;
				float test852;
				float test941;
				float test1209;
				float test1336;
				float test1477;
				float test1633;
				sf::RectangleShape rectangel;
				rectangel.rotate(180);
				rectangel.setFillColor(sf::Color::Green);
				rectangel.setSize({ 1,1 });
				rectangel.setPosition({ 100,1000 });
				std::vector<float> værdier;

					sf::RenderWindow goertzel(sf::VideoMode(500, 500, 32), "SFML Goertzel");
					while (goertzel.isOpen())
					{
						sf::Event event;
						while (goertzel.pollEvent(event))
						{
							if (event.type == sf::Event::Closed)
								goertzel.close();
						}


						recorder.start(44100);
						Sleep(99);
						recorder.stop();

						test697 = goertzel_mag(recorder.getBuffer().getSampleCount(), 697, 44100, recorder.getBuffer().getSamples());
						værdier.push_back(test697);
						test770 = goertzel_mag(recorder.getBuffer().getSampleCount(), 770, 44100, recorder.getBuffer().getSamples());
						værdier.push_back(test770);
						test852 = goertzel_mag(recorder.getBuffer().getSampleCount(), 852, 44100, recorder.getBuffer().getSamples());
						værdier.push_back(test852);
						test941 = goertzel_mag(recorder.getBuffer().getSampleCount(), 941, 44100, recorder.getBuffer().getSamples());
						værdier.push_back(test941);
						test1209 = goertzel_mag(recorder.getBuffer().getSampleCount(), 1209, 44100, recorder.getBuffer().getSamples());
						værdier.push_back(test1209);
						test1336 = goertzel_mag(recorder.getBuffer().getSampleCount(), 1336, 44100, recorder.getBuffer().getSamples());
						værdier.push_back(test1336);
						test1477 = goertzel_mag(recorder.getBuffer().getSampleCount(), 1477, 44100, recorder.getBuffer().getSamples());
						værdier.push_back(test1477);
						test1633 = goertzel_mag(recorder.getBuffer().getSampleCount(), 1633, 44100, recorder.getBuffer().getSamples());
						værdier.push_back(test1633);


						//int testGoertzel = 40;
						//float value;
						//while (testGoertzel < 1840)
						//{
						//	value = 0;
						//	for (int i = testGoertzel; i <= testGoertzel + 5; i++)
						//	{
						//		value +=goertzel_mag(recorder.getBuffer().getSampleCount(), i, 20000, recorder.getBuffer().getSamples());
						//	}
						//	værdier.push_back(value);
						//	testGoertzel = testGoertzel + 5;
						//}
						//
						//testGoertzel = 0;
						//


						for (int i = 0; i < værdier.size(); i++)
						{
							rectangel.setPosition({40+float(i)*float(500./værdier.size()),490 });
							rectangel.setSize({ 20,2+værdier[i]/float(8)});
							rectangel.setFillColor(sf::Color(255 - 255 * (i / float(værdier.size())), 255 * (i / float(værdier.size())),0, 255));
							goertzel.draw(rectangel);

						}
						goertzel.display();
						goertzel.clear();
						værdier.clear();

						
					}

					 
				}

		}
		if (choice == 'p')  //Plot samples
		{

			sound.play();

			while (sound.getStatus() == sf::Sound::Playing)
			{
				std::cout << "\rPlaying... " << sound.getPlayingOffset().asSeconds() << " sec        ";
				std::cout << std::flush;

				sf::sleep(sf::milliseconds(100));
			}
		}
		else if (choice == 'v')
		{
			char choicev;
			std::cout << "What do you want to do with the plot (c = constant update, s = singel update) ? ";
			std::cin >> choicev;
			std::cin.ignore(10000, '\n');

			int maxamp = 0;
			int minamp = 0;
			std::vector<int> finalsampels;

			for (int i = 0; i < int(buffer.getSampleCount()); i++)
			{
				if (int(buffer.getSamples()[i]) > maxamp)
					maxamp = int(buffer.getSamples()[i]);
				if (int(buffer.getSamples()[i]) < minamp && int(buffer.getSamples()[i]) > -30000)
					minamp = int(buffer.getSamples()[i]);

			}
			std::cout << "max amp: " << maxamp << "   min amp: " << minamp;
			for (int i = 0; i < int(buffer.getSampleCount()); i++)
			{
				finalsampels.push_back(int(buffer.getSamples()[i] - minamp));
				//std::cout << finalsampels[i] << " ";

			}

			int maxhight = maxamp - minamp;
			std::cout << maxhight << std::endl;

			sf::RenderWindow App(sf::VideoMode(1800, 1000, 32), "SFML Graphics");
			sf::CircleShape shape;
			shape.setFillColor(sf::Color::Red);
			shape.setRadius(1);
			// Start game loop

			bool firstrun = true;

			if (choicev == 'c')
			{
				minamp = -8000;
				maxhight = 16000;
				
				while (App.isOpen())
				{

					sf::Event event;
					while (App.pollEvent(event))
					{
						if (event.type == sf::Event::Closed)
							App.close();
					}
					recorder.start(44100);
					Sleep(200);
					recorder.stop();
					finalsampels.clear();
					for (int i = 0; i < int(recorder.getBuffer().getSampleCount()); i++)
					{
						finalsampels.push_back(int(recorder.getBuffer().getSamples()[i] - minamp));
						//std::cout << finalsampels[i] << " ";

					}
					

					for (int i = 0; i < finalsampels.size(); i++)
					{
						shape.setPosition({ float(1800. * (float(i) / float(finalsampels.size()))),float(1000 * (float(finalsampels[i]) / float(maxhight))) });

						if (1000*float(finalsampels[i]) / float(maxhight) < 400 || 1000 * float(finalsampels[i]) / float(maxhight) > 600)
						{
							shape.setOutlineColor(sf::Color::Red);
							shape.setFillColor(sf::Color::Red);
						}
						else
						{
							shape.setOutlineColor(sf::Color::Green);
							shape.setFillColor(sf::Color::Green);
						}
						
						App.draw(shape);
					}
					//std::cout << finalsampels.size() << std::endl;
					App.display();
					App.clear();
				}
			}
			else
			{
				while (App.isOpen())
				{
					sf::Event event;
					while (App.pollEvent(event))
					{
						if (event.type == sf::Event::Closed)
							App.close();
					}
					if (firstrun)
					{
						for (int i = 0; i < finalsampels.size(); i++)
						{
							shape.setPosition({ float(1800. * (float(i) / float(finalsampels.size()))),float(1000 * ((finalsampels[i]) / float(maxhight))) });
							App.draw(shape);
						}
						App.display();
						App.clear();
						firstrun = false;
					}

				}
			}



		}


		std::cout << std::endl << "Done!" << std::endl;

		std::cout << "Press enter to restart.." << std::endl;
		std::cin.ignore(10000, '\n');

	}
}