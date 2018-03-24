#include "utility.h"
#include "timing.h"
#include <iostream>
#include "progress.h"
#include "interpret.h"
#include "function.hpp"

int main(int argc, char** argv){
    auto arglist=live_interpret(argc, argv);
    palindromic_factor(arglist);
    return 0;
}
