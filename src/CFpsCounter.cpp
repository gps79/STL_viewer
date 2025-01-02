/**
 * @file CFpsCounter.cpp
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#include "CFpsCounter.h"

void CFpsCounter::nextFrame()
{
    constexpr float OneSecond{1.0f};
    constexpr float QuarterSecond{0.25f};
    constexpr uint32_t MinNumFramesToCalcFps{25};

    ++m_u32Frames;
    float fCurrentTimeSeconds = static_cast<float>(GetTickCount()) / 1000.0f;
    if (((m_u32Frames >= MinNumFramesToCalcFps) && ((fCurrentTimeSeconds - m_fStartTimeSeconds) > QuarterSecond))
         || ((fCurrentTimeSeconds - m_fStartTimeSeconds) > OneSecond))
    {
        m_fFps = static_cast<float>(m_u32Frames) / (fCurrentTimeSeconds - m_fStartTimeSeconds);
        m_fStartTimeSeconds = fCurrentTimeSeconds;
        m_u32Frames = 0;
    }
}
