#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

//// Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Defines
#define LOG(x) std::cout << x << std::endl;

// Variables
extern std::unordered_map<std::string, void(*)(std::string args)> commandMap;

// Functions
void	CLI();
void	executeCommand(std::string input);
void	help(std::string args);