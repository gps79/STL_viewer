/**
 * @file CFpsCounter.h
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#ifndef STL_VIEWER_CFPSCOUNTER_H_INCLUDED
#define STL_VIEWER_CFPSCOUNTER_H_INCLUDED

#include <windows.h>
#include <stdint.h>

/**
 * @class CFpsCounter
 * @brief A class to calculate and track the frames per second (FPS) of the application.
 *
 * The CFpsCounter class provides methods to track the number of frames rendered in a given time
 * period, allowing the application to calculate the frames per second (FPS). It is useful for
 * performance monitoring and displaying FPS information during runtime.
 */
class CFpsCounter
{
public:
    /**
     * @brief Updates the frame count and calculates the FPS.
     *
     * This function should be called once per frame. It updates the frame counter
     * and computes the current frames per second (FPS) based on the time elapsed.
     */
    void nextFrame();

    /**
     * @brief Gets the current FPS (frames per second).
     *
     * This function returns the calculated FPS, which represents the number of frames
     * rendered per second.
     *
     * @return The current FPS value.
     */
    float getFps() const { return m_fFps; }

private:
    uint32_t m_u32Frames{0}; ///< The number of frames rendered.
    float m_fStartTimeSeconds{static_cast<float>(GetTickCount()) / 1000.0f}; ///< The start time in seconds when the FPS calculation begins.
    float m_fFps{0}; ///< The current FPS value.
};

#endif // STL_VIEWER_CFPSCOUNTER_H_INCLUDED
