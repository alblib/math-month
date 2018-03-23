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

using namespace std;
using namespace boost::multiprecision;

mutex print_mutex;

array<uint256_t, 34> primes = {1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, \
67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137};
uint256_t thr = uint256_t(5521159517ull)*uint256_t(1000000000000ull) + uint256_t(777159511255ull);




timing_measurement<> main_timing = timing_measurement<>();

vector<uint256_t> functor24(){
    vector<uint256_t> factor24;
    for(uint32_t i = 0; i < (1ul << 24); ++i){
        uint32_t temp = i;
        uint256_t result=primes[33];
        for (char j=0;j<24;++j){
            if (temp&1) result*=primes[j+1];
            temp>>=1;
        }
        factor24.push_back(result);
    }
    std::sort(factor24.begin(),factor24.end());
    return factor24;
}

vector<uint256_t> factor24=functor24();

std::vector<uint256_t> block(uint16_t index, const vector<uint256_t>& factor24){
    auto id = index;
    print_mutex.lock();
     cout << "thread " << id << " ["
     << std::chrono::duration<double>(main_timing.real_time()).count()
     << "s] loaded"<<endl;
     print_mutex.unlock();
    uint256_t index_factor = 1;
    for(int i = 0; i < 8; ++i){
        index_factor *= ((index & 1) ? primes[25+i]: 1);
        index >>= 1;
    }
    uint256_t comp_thr = ::thr/index_factor;
    auto it = factor24.begin();
    while (it != factor24.end()){
        if (*it > comp_thr) break;
        ++it;
    }
    std::vector<uint256_t> result;
    while (it != factor24.end()){
        uint256_t num = ((*it) * index_factor);
        if (is_palindrome(num)) 
            result.push_back(num);
        ++it;
    }
    print_mutex.lock();
    cout << "thread " << id << " ["
     << std::chrono::duration<double>(main_timing.real_time()).count()
     << "s] ended"<<endl;
     print_mutex.unlock();
    return result;
}

void block_thread(uint16_t index, const std::vector<uint256_t>& factor24, 
    std::vector<uint256_t>& result, std::mutex& result_mutex){
    for(size_t i = 0; i < 8; ++i){
        auto result_temp = block((index<<3)+i, factor24);
        result_mutex.lock();
        result.insert(result.end(), result_temp.begin(), result_temp.end());
        result_mutex.unlock();
    }
}



int main(){
    cout << std::chrono::duration<double>(main_timing.real_time()).count() << "s: factor 22 ready." << endl;
    
    std::vector<uint256_t> result;
    
    std::mutex result_mutex;
    std::vector<thread> threads;
    
    for (size_t i = 0; i < 32; ++i)
        threads.emplace_back(block_thread,i,std::ref(factor24),std::ref(result),std::ref(result_mutex));
    
    for (size_t i = 0; i <32; ++i)
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
