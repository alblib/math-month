#include "progress.h"
#include <iostream>
#include <chrono>

progress_bar::progress_bar(size_t total) noexcept:
total(total), timer(),progress(0),current_star(0){
    std::cout << "          0%   10   20   30   40   50   60   70   80   90   100%";
    std::cout << std::endl;
    std::cout << "          |----|----|----|----|----|----|----|----|----|----|" << std::endl;
}

void progress_bar::refresh_bar() noexcept{
    int newstar = double(progress)/double(total)*50;
    //if (current_star == newstar && newstar != 0) return;
    if (progress == 0) return;
    if (progress == total){
        print_mutex.lock();
        std::cout << "\r";
        std::cout << timer.real_time_print();
        std::cout << " *************************************************** [complete]";
        std::cout << std::endl;
        print_mutex.unlock();
        std::cout << std::flush;
        return;
    }
    std::string past = timer.real_time_print();
    std::string timin = timing_measurement<>::formatted(
            std::chrono::duration<double>(
            std::chrono::duration_cast<std::chrono::duration<double>>(timer.real_time()).count()*
            double(total-progress)/double(progress)));
    print_mutex.lock();
    std::cout <<"\r" << past << " *";
    size_t  i = 0;
    for (; i <newstar; ++i)
        std::cout << "*";
    for (; i < 52; ++i)
        std::cout << " ";
        std::cout << timin << " " << progress;
    print_mutex.unlock();
    std::cout << std::flush;
}

progress_bar &progress_bar::operator=(size_t progress) noexcept {
    this->progress = progress;
    refresh_bar();
    return *this;
}
progress_bar &progress_bar::operator+=(size_t progress) noexcept {
    this->progress += progress;
    refresh_bar();
    return *this;
}

progress_bar &progress_bar::operator++() noexcept {
    ++progress;
    refresh_bar();
    return *this;
}


