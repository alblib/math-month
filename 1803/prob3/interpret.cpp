#include "interpret.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

std::vector<int> interpret(int argc, char ** argv)
{
	std::vector<int> result;
	for (int i = 1; i < argc; ++i)
		result.push_back(std::stoi(argv[i]));
	return result;
}

std::vector<int> interpret(char * line)
{
	std::vector<int> result;
	std::stringstream ss;
	ss << line;
	int i;
	while (ss >> i)
		result.push_back(i);
	return result;
}

std::vector<int> live_interpret(int argc, char ** argv)
{
    std::cout << "/********************************************************************/" << std::endl;
    std::cout << "/* March 2018, Problem #3. What is the largest palindromic factor   */" << std::endl;
    std::cout << "/*                         of Product[Prime[i],{i,n}] for given n?  */" << std::endl;
    std::cout << "/********************************************************************/" << std::endl;

	std::vector<int> result;
	if (argc >= 2) result = interpret(argc, argv);
	else {
		std::cout << "Input n's: ";
		char buffer[2048];
		std::cin.getline(buffer, 2047);
		result = interpret(buffer);
	}
	std::sort(result.begin(), result.end());
	std::cout << std::endl << "Input n's : ";
	for (size_t i = 0; i < result.size(); ++i) {
		std::cout << result[i];
		if (i != result.size() - 1) std::cout << ", ";
	}
	std::cout << std::endl;
	return result;
}
