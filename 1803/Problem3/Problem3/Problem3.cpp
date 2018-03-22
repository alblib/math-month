// Problem3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Math.h"
#include "timing.h"
#include <boost/multiprecision/cpp_int.hpp>

int main(int argc, char** argv)
{
	//auto param_list = live_interpret(argc, argv);
	timing_measurement<> timer;
	prime_product_divisors<boost::multiprecision::uint256_t, uint32_t>(24);
	std::cout << timer.real_time_print() << std::endl;
    return 0;
}

