//
// Created by gorev on 14.08.2025.
//

#include "Engine/Timer.h"

Timer::Timer()
{
    start = std::chrono::high_resolution_clock::now();
    stop = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
}

double Timer::GetMilliseconds() const
{
    if (m_isRunning)
    {
        auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
        return elapsed.count();
    }
    else
    {
        auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
        return elapsed.count();
    }
}

void Timer::Restart()
{
    m_isRunning = true;
    start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop()
{
    if (!m_isRunning)
        return false;

    stop = std::chrono::high_resolution_clock::now();
    m_isRunning = false;

    return true;
}

bool Timer::Start()
{
    if (m_isRunning)
        return false;

    start = std::chrono::high_resolution_clock::now();
    m_isRunning = true;

    return true;
}
