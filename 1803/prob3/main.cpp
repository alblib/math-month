#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <array>
#include <chrono>
#include <type_traits>
#include <functional>
#include <algorithm>
#include <sstream>
#include <valarray>
#include <thread>
#include <mutex>
#include <fstream>

#include "utility.h"
#include "timing.h"
#include <boost/progress.hpp>

using namespace std;
using namespace boost::multiprecision;

mutex print_mutex;

size_t done_thr = 0;

uint256_t thr = uint256_t(5521159517ull)*uint256_t(1000000000000ull) + uint256_t(777159511255ull);

auto primes = prime_array(34);


timing_measurement<> main_timing = timing_measurement<>();


vector<uint256_t> factor24=prime_product_divisors_multithread(24);

uint256_t block(uint64_t index, const vector<uint256_t>& factor24, const uint256_t max_hint){
    auto id = index;

    uint256_t index_factor = primes[34];
    for(int i = 0; i < 9; ++i){
        index_factor *= ((index & 1) ? primes[25+i]: 1);
        index >>= 1;
    }
    uint256_t max = max_hint;
    uint256_t temp;
    auto it = factor24.begin();
    while (it != factor24.end()){
        temp = *it * index_factor;
        if (temp > max && is_palindrome(temp))
            max=temp;
        ++it;
    }

    return max;
}

void block_thread(uint64_t index, const std::vector<uint256_t>& factor24, 
    std::vector<uint256_t>& result, std::mutex& result_mutex){
        

    uint256_t max = ::thr;
    for(size_t i = 0; i < 32; ++i){
        max = std::max(block((i<<4)+index, factor24, max),max);
        if (max > ::thr){
            result_mutex.lock();
            result.insert(result.end(), max);
            result_mutex.unlock();
        }
    }
}



int main(){
    cout << std::chrono::duration<double>(main_timing.real_time()).count() << "s: factor 24 ready." << endl;
    print_mutex.lock();
    cout << "a\rb";
    boost::progress_display dis(512);
print_mutex.unlock();
    std::vector<uint256_t> result;
    
    std::mutex result_mutex;
    std::vector<thread> threads;
    
    for (size_t i = 0; i < 16; ++i)
        threads.emplace_back(block_thread,i,std::ref(factor24),std::ref(result),std::ref(result_mutex));
    
    for (size_t i = 0; i <16; ++i)
        threads[i].join();
    /*
    for (size_t i = 0; i < 128; ++i){
        auto temp = block(i,factor22);
        result.insert(result.end(),temp.begin(), temp.end());
    }*/
    cout << "end" << endl;
    std::sort(result.begin(), result.end());
    ofstream ifs("output.txt");
    for (const uint256_t& v: result)
        ifs << v << endl;
    ifs.close();
    return 0;
}
