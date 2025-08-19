//
// Created by gorev on 14.08.2025.
//

#ifndef TIMER_H
#define TIMER_H
#include <chrono>


class Timer
{
public:
    Timer();

    ~Timer();

    double GetMilliseconds() const;

    void Restart();

    bool Stop();

    bool Start();

private:
    bool m_isRunning = false;

    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> stop;
};


#endif //TIMER_H
