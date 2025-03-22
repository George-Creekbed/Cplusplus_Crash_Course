#include <sys/time.h>
#include <cstdio>
#include <utility>
#include <cmath>

struct TimerClass {
    TimerClass(): name{"Generic timer"} {
        gettimeofday(&timestamp, nullptr);
        start_time = timestampInSeconds();
    }

    TimerClass(const char* input): name{input} {
        gettimeofday(&timestamp, nullptr);
        start_time = timestampInSeconds();
    }

    timeval getTimestamp() const {
        return timestamp;
    }

    const char* getName() {
        return name;
    }

    double timestampInSeconds() const {
        return timestamp.tv_sec + timestamp.tv_usec * 1E-06;
    }

    ~TimerClass() {
        double age = timestampInSeconds() - start_time;
        if (timestampInSeconds())
            printf("Age of timer %s at destruction: %lg seconds\n", name, age);
    }

    TimerClass(const TimerClass& source): timestamp{source.getTimestamp()}, name{source.name} {
        start_time = timestampInSeconds();
    }

    TimerClass& operator=(const TimerClass& source) {
        if (this == &source)
            return *this;
        timestamp = source.getTimestamp();
        start_time = timestampInSeconds();
        name = source.name;

        return *this;
    }

    void resetTimestamp() {
        timestamp = {};
    }

    TimerClass(TimerClass&& source) noexcept: 
    start_time{source.start_time}, timestamp{source.getTimestamp()}, name{source.name} {
        source.start_time = {};
        source.resetTimestamp();
        source.name = nullptr;
    }

    TimerClass& operator=(TimerClass&& source) noexcept {
        if (this == &source)
            return *this;
        start_time = source.start_time;
        timestamp = source.getTimestamp();
        name = source.name;
        source.start_time = {};
        source.resetTimestamp();
        source.name = nullptr;

        return *this;
    }

    double someMath() {
        double a{timestampInSeconds()}, temp{};
        for (size_t i=0; i!=1'000; ++i) 
            temp += pow(erf(sqrt(pow(exp(cos(a)), 2.0) / log(a))), i);
        
        return temp;
    }

private:
    struct timeval timestamp;
    double start_time;
    const char* name;
};

int main(){
    TimerClass my_timer{"A"};
    printf("Timer %s at construction: seconds=%lf\n", my_timer.getName(), my_timer.timestampInSeconds());
    double r{my_timer.someMath()};
    printf("Some math result = %f\n", r);
    printf("Copy ...\n");
    TimerClass my_timer_copy{my_timer};
    printf("Original timer %s, timestamp: %lf\n", my_timer.getName(), my_timer.timestampInSeconds());
    printf("Copied timer %s, timestamp: %lf\n", my_timer_copy.getName(), my_timer_copy.timestampInSeconds());
    TimerClass third_timer{"C"};
    r = third_timer.someMath();
    printf("Again some math result = %f\n", r);
    printf("While clock time is at, %lf\n", third_timer.timestampInSeconds());
    third_timer = std::move(my_timer_copy);
    printf("Moved timer %s, timestamp: %lf\n", third_timer.getName(), third_timer.timestampInSeconds());
    printf("Moved-from copied timer %s, timestamp: %lf\n", my_timer_copy.getName(), my_timer_copy.timestampInSeconds());
}