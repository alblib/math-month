#include <chrono>
#include <cstdio>
#include <string>
#include <boost/format.hpp>

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
public:
    constexpr timing_measurement() noexcept:
        origin_clock(clock()), steady_origin(std::chrono::steady_clock::now()){}
    
    void reset() noexcept{
        *this = timing_measurement();
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
	std::string real_time_print() const noexcept {
		std::stringstream ss;
		ss << "["
        << boost::format("%1$0.4f") % std::chrono::duration<double>(real_time()).count() << " s]";
		return ss.str();
	}
};
