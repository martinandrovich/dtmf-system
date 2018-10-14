#include <SFML\Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <vector>
#include <cmath>
#include <iostream>


sf::SoundBuffer makeSound(int f1, int f2, int samplef)
{
	const int samples = 441000;
	const double fade = 5000;
	sf::Int16 temp[samples];
	sf::SoundBuffer tempSoundbuffer;
	int ampMax = 15000;
	int ampFinal;
	
	for (unsigned i = 0; i < samples-1; i++)
	{
		if (i < fade) {
			ampFinal = (i / fade) * ampMax;
		}
		else if (i > samples-fade) {
			ampFinal =((samples - i) / (fade)) * ampMax;
		}
		else
		{
			ampFinal = ampMax;
		}
		temp[i] = ampFinal * (sin(f1 * 2 * 3.141592653589732*(i * 1. / samplef)) + sin(f2 * 2 * 3.141592653589732*(i * 1. / samplef)));

	}
	tempSoundbuffer.loadFromSamples(temp, samples, 1, samplef);
	return tempSoundbuffer;

}

int main()
{
	if (!sf::SoundBufferRecorder::isAvailable())
	{
		std::cout << " error: audio capture is not available on this system" << std::endl;
	}

	sf::SoundBufferRecorder recorder;
	sf::Sound sound;
	sf::SoundBuffer buffer;
	sf::SoundBuffer buffer1;

	std::cout << " Type R to start recording" << std::endl;
	std::cout << " Type S to stop recording" << std::endl;
	std::cout << " Type P to play recording" << std::endl;
	std::cout << " Type T to Terminate recording" << std::endl;

	int samplef = 44100;
	const int Amp = 15000;
	const sf::Uint64 samples = 44100;
	int f1;
	int f2;

	sf::Int16 one[samples];



	while (true)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
		{
			f1 = 697;
			f2 = 1209;
			buffer1 = makeSound(f1, f2, samplef);
			sound.setBuffer(buffer1);
			sound.play();
			Sleep(1000);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
		{
			f1 = 697;
			f2 = 1336;
			buffer1 = makeSound(f1, f2, samplef);
			sound.setBuffer(buffer1);
			sound.play();
			Sleep(1000);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
		{
			f1 = 697;
			f2 = 1477;
			buffer1 = makeSound(f1, f2, samplef);
			sound.setBuffer(buffer1);
			sound.play();
			Sleep(1000);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
		{
			f1 = 770;
			f2 = 1209;
			buffer1 = makeSound(f1, f2, samplef);
			sound.setBuffer(buffer1);
			sound.play();
			Sleep(1000);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			recorder.start(44100);
			std::cout << "start recording" << std::endl;
			Sleep(200);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			recorder.stop();
			buffer = recorder.getBuffer();
			sound.setBuffer(buffer);
			std::cout << "stop recording" << std::endl;
			Sleep(200);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		{
			sound.play();
			std::cout << "sound is playing" << std::endl;
			Sleep(200);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		{
			sound.stop();
			std::cout << "sound is stoped" << std::endl;
			Sleep(200);
		}


	}

}