//
//  function.cpp
//  mmth1803
//
//  Created by WOOHYUN RIM on 2018-03-24.
//  Copyright © 2018 Protelum Liberium. All rights reserved.
//

#include "function.hpp"
#include "utility.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include "progress.h"
#include "atomic_types.h"


boost::multiprecision::uint256_t known30= boost::multiprecision::uint256_t(5521159517ull)*boost::multiprecision::uint256_t(1000000000000ull) + boost::multiprecision::uint256_t(777159511255ull);

/** known maximal palindromic factor of a product of n prime numbers, listed by index n. */
std::vector<boost::multiprecision::uint256_t> palindrom_knowns={
    1, 2, 6, 6, 7, 77, 6006, 6006, 969969, 969969, 969969,
    36399363, 1346776431, 1346776431, 1346776431, 5330660335ull, 5330660335ull, 5330660335ull,
    5041813181405ull, 5041813181405ull, 5041813181405ull, 5041813181405ull,
    74833599533847ull, 74833599533847ull, 74833599533847ull, 74833599533847ull,
    74833599533847ull, 74833599533847ull, 74833599533847ull,
    502073167761370205ull, known30, known30, known30, known30, known30
};

void pal_single(std::uint_fast8_t n, std::uint_fast8_t index, atomic<boost::multiprecision::uint256_t>& result, const std::vector<boost::multiprecision::uint256_t>& factor24, const std::vector<boost::multiprecision::uint256_t>& primen){
    boost::multiprecision::uint256_t temp_Result = result;
    boost::multiprecision::uint256_t index_factor = primen[n];
    for (int i = 0; i < n - 25; ++i){
        index_factor *= ((index & 1) ? primen[25+i]: 1);
        index >>= 1;
    }
    boost::multiprecision::uint256_t temp;
    auto it = factor24.begin();
    while (it != factor24.end()){
        temp = *it * index_factor;
        if (temp > temp_Result && is_palindrome(temp))
            temp_Result=temp;
        ++it;
    }
    result.lock();
    if (result < temp_Result) result.raw_assign(temp_Result);
    result.unlock();
}

void pal_singlethread4(std::uint_fast8_t n, std::uint_fast8_t thr_index, atomic<boost::multiprecision::uint256_t>& result, const std::vector<boost::multiprecision::uint256_t>& factor24, const std::vector<boost::multiprecision::uint256_t>& primen, progress_bar& bar){
    std::uint64_t index_per_thread = std::uint64_t(1) << (n-27); //-24-2(2^2) -1(for prime[last])
    for(std::uint64_t index = thr_index++*index_per_thread;
        index < thr_index*index_per_thread; ++index){
        ++bar;
        pal_single(n,index,result,factor24,primen);
        ++bar;
    }
}

boost::multiprecision::uint256_t pal_thread(std::uint_fast8_t n, const std::vector<boost::multiprecision::uint256_t>& factor24, const std::vector<boost::multiprecision::uint256_t>& primen){
    progress_bar bar(std::uint64_t(1)<<(n-24));
    atomic<boost::multiprecision::uint256_t> result = palindrom_knowns.back();
    std::thread thr1(pal_singlethread4, n, 0, std::ref(result), std::ref(factor24), std::ref(primen), std::ref(bar));
    std::thread thr2(pal_singlethread4, n, 1, std::ref(result), std::ref(factor24), std::ref(primen), std::ref(bar));
    std::thread thr3(pal_singlethread4, n, 2, std::ref(result), std::ref(factor24), std::ref(primen), std::ref(bar));
    std::thread thr4(pal_singlethread4, n, 3, std::ref(result), std::ref(factor24), std::ref(primen), std::ref(bar));
    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
    return result;
}


void palindromic_factor(std::vector<int> n){
    std::sort(n.begin(),n.end());
    size_t t = 0;
    while (n[t] < palindrom_knowns.size() && t < n.size()){
        std::cout << "n=" << n[t] << ": result= " << palindrom_knowns[n[t]]<<std::endl << std::endl;
        ++t;
    }
    auto maxn = n.back();
    if (maxn < palindrom_knowns.size()) return;
    n.clear();
    n.reserve(maxn - palindrom_knowns.size());
    for (int i = static_cast<int>(palindrom_knowns.size()); i<=maxn; ++i)
        n.push_back(i);
    
    std::cout << "Acquiring All Divisors of Prime Product 24." << std::endl;
    auto factor24 = prime_product_divisors_progressbar(24);
    auto primen = prime_array(n.back());
    std::cout << std::endl;
    for(const int& ncur: n){
        std::cout << "n=" << ncur << ": calculating..." << std::endl;
        auto result= pal_thread(ncur,factor24,primen);
        std::cout << "result= " << result;
        std::cout << std::endl << std::endl;
        palindrom_knowns.push_back(result);
    }
}
