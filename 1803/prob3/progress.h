#include "timing.h"
#include <boost/progress.hpp>
#include <atomic>
#include <mutex>

class progress_bar{
    timing_measurement<> timer;
    std::atomic<size_t> progress;
    std::atomic<size_t> total;
    
    std::atomic<size_t> current_star;
    std::atomic<size_t> next_star;
    
    std::mutex print_mutex;
    
    void refresh_bar() noexcept;
public:
    progress_bar(size_t total) noexcept;
    progress_bar& operator = (size_t progress) noexcept;
    progress_bar& operator += (size_t progress) noexcept;
    progress_bar& operator ++ () noexcept;
};
