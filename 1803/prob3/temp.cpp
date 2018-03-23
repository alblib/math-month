#include "utility.h"
#include "timing.h"
#include <iostream>

int main(){
    using namespace boost::multiprecision;
    timing_measurement<> main_timing = timing_measurement<>();
    std::cout << prime_product_divisors_multithread(24).size() << std::endl;
    //prime_product_array<uint256_t>(24);
    std::cout << main_timing.real_time_print() << std::endl;
    return 0;
}
