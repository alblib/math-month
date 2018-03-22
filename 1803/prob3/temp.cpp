#include "utility.h"
#include "timing.h"
#include <iostream>

int main(){
    using namespace boost::multiprecision;
    timing_measurement<> main_timing = timing_measurement<>();
    //prime_product_divisors<uint256_t, uint32_t>(24);
    prime_product_array<uint256_t>(24);
    std::cout << main_timing.real_time_print();
    return 0;
}