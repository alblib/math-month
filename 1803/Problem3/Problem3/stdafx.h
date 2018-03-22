// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef __WIN32

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#endif

#include <vector>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>

// TODO: reference additional headers your program requires here

/** interpret command line with arguments */
std::vector<int> interpret(int argc, char** argv);
/** interpret argument list input */
std::vector<int> interpret(char* line);

std::vector<int> live_interpret(int argc, char** argv);
