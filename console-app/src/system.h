#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>

//// Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Defines
#define LOG(x) std::cout << x << std::endl;

// Variables
extern std::unordered_map<std::string, void(*)(std::string args)> commandMap;

// Functions
void	initCLI();
void	initSystem(std::string args);
void	runGame();

void	pressKey(int key, int duration, bool parallel = true);
int		listenForKey();
void	clientWork();
void	executePayload(int toneId, int clientId);

void	CLI();
void	executeCommand(std::string input);
void	help(std::string args);