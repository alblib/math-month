#pragma once

#include <chrono>
#include <cstdio>
#include <string>
#include <boost/format.hpp>
#include <cmath>
#include <sstream>

template <typename TimeT, typename ResultT = void>
struct timing_result_type{
    TimeT cpu_time, real_time;
    ResultT result;
};

template <typename TimeT>
struct timing_result_type<TimeT, void>{
    TimeT cpu_time, real_time;
};


template<typename TimeT = std::chrono::duration<double>>
struct timing_measurement
{
public:
    template <typename ResultT>
    using result_type = timing_result_type<TimeT, ResultT>;

private:
    template <class T>
    static result_type<typename std::remove_reference<T>::type> make_result(T&& result){
        result_type<typename std::remove_reference<T>::type> go;
        go.result = result;
        return go;
    }
    static result_type<void> make_result(void){
        result_type<void> go;
        return go;
    }
public:
    template<typename F, typename ...Args>
    static auto execution(F&& func, Args&&... args) -> result_type<typename std::function<typename std::remove_reference<F>::type>::result_type>
    {
        auto start = std::chrono::steady_clock::now();
        std::clock_t begin = std::clock();
        auto result = make_result(func(std::forward<Args>(args)...));
        std::clock_t end = std::clock();
        auto finish = std::chrono::steady_clock::now();
        result.real_time = std::chrono::duration_cast<TimeT>(finish - start);
        result.cpu_time = std::chrono::duration<int64_t,std::ratio<1,CLOCKS_PER_SEC>>(end-begin);
        return result;
    }
private:
    std::clock_t origin_clock;
    std::chrono::steady_clock::time_point steady_origin;
    std::clock_t saved_clock; // 0
    std::chrono::steady_clock::duration saved_interval; // 0
public:
    constexpr timing_measurement() noexcept:
    origin_clock(std::clock()), steady_origin(std::chrono::steady_clock::now()),
        saved_clock(0) , saved_interval(0)
    {}
    
    inline constexpr bool is_running() const{
        return origin_clock == 0;
    }
    void start() noexcept{
        if (!is_running()){
            origin_clock = std::clock();
            steady_origin = std::chrono::steady_clock::now();
        }
    }
    void stop() noexcept{
        saved_clock += std::clock() - origin_clock;
        saved_interval += std::chrono::steady_clock::now() - steady_origin;
        origin_clock = 0;
        steady_origin = decltype(steady_origin)(0);
    }
    
    void reset() noexcept{
        saved_clock = 0;
        saved_interval = decltype(saved_interval)(0);
        origin_clock = 0;
        steady_origin = decltype(steady_origin)(0);
    }
    std::clock_t clock() const noexcept{
        return std::clock() - origin_clock;
    }
    std::chrono::duration<int64_t,std::ratio<1,CLOCKS_PER_SEC>> cpu_time() const noexcept{
        return std::chrono::duration<int64_t,std::ratio<1,CLOCKS_PER_SEC>>(this->clock());
    }
    std::chrono::steady_clock::duration real_time() const noexcept{
        return std::chrono::steady_clock::now() - steady_origin;
    }
    template <class R1, class R2>
    static std::string formatted(const std::chrono::duration<R1,R2>& dur){
        auto val = std::chrono::duration_cast<std::chrono::duration<long double>>(dur).count();
        std::stringstream ss;
        ss << "[";
        if (val < 10){              // 0.0000s
            ss << boost::format("%1$0.4f") % val << "s";
        }else if (val < 60){        // 00.000s
            ss << boost::format("%1$0.3f") % val << "s";
        }else if (val < 3600){      // 00m 00s
            int min = std::ceil(val/60), sec = val - int(val/60)*60;
            ss << boost::format("%1$02d") % min << "m ";
            ss << boost::format("%1$02d") % sec << "s";
        }else if (val < 30*3600) {  // 00h 00m
            int min = std::ceil(val/60);
            int hr = min / 60;
            min %= 60;
            ss << boost::format("%1$02d") % hr << "h ";
            ss << boost::format("%1$02d") % min << "m";
        }else {                     // 00d 00h
            int hr = std::ceil(val/3600);
            int d = hr / 24;
            hr %= 24;
            ss << boost::format("%1$02d") % d << "d ";
            ss << boost::format("%1$02d") % hr << "h";
        }
        ss << "]";
        return ss.str();
    }
    std::string real_time_print() const noexcept {
        return formatted(real_time());
    }
};
