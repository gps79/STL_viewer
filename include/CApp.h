/**
 * @file CApp.h
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#ifndef STL_VIEWER_CAPP_H_INCLUDED
#define STL_VIEWER_CAPP_H_INCLUDED

#include <windows.h>
#include <stdlib.h>
#include "common.h"
#include "CModel.h"
#include "CRenderer.h"

/**
 * @class CApp
 * @brief The main application class for handling the 3D model viewer.
 *
 * This class implements the core functionality of the application, including initialization,
 * running the application, handling user input, and managing the window's message loop.
 * It is designed as a singleton to ensure there is only one instance of the application.
 */
class CApp
{
public:
    /**
     * @brief Deleted copy constructor to prevent copying of the singleton instance.
     */
    CApp(const CApp &) = delete;

    /**
     * @brief Deleted assignment operator to prevent assigning the singleton instance.
     */
    void operator=(const CApp&) = delete;

    /**
     * @brief Gets the singleton instance of the application.
     *
     * This function ensures that there is only one instance of the application.
     * It returns a reference to the unique instance.
     *
     * @return A reference to the unique CApp instance.
     */
    static CApp &getInstance()
    {
        static CApp appInstance; ///< Static instance of the singleton.
        return appInstance;
    }

    /**
     * @brief Parses command line arguments.
     *
     * This function processes the command line arguments passed to the application
     * during initialization.
     *
     * @return An error code indicating the result of the operation.
     */
    Err getCmdLineArguments();

    /**
     * @brief Handles error codes and displays appropriate messages.
     *
     * This function is called to handle various error codes by displaying corresponding
     * error messages to the user.
     *
     * @param errorCode The error code to handle.
     */
    void handleErrorCode(Err errorCode) const;

    /**
     * @brief Initializes the application.
     *
     * This function initializes the application, including creating application window, setting up the renderer
     * and loading the STL file passed as a command line argument.
     *
     * @return An error code indicating the result of the initialization.
     */
    Err init();

    /**
     * @brief Runs the main application loop.
     *
     * This function runs the main event loop, processing input, rendering the scene,
     * and handling messages.
     *
     * @return An error code indicating the result of the operation.
     */
    Err run();

    /**
     * @brief Message handler for the application window.
     *
     * This function processes messages sent to the window, such as input events
     * and system messages.
     *
     * @param hWnd The handle to the window receiving the message.
     * @param uMsg The message identifier.
     * @param wParam Additional message-specific information.
     * @param lParam Additional message-specific information.
     *
     * @return The result of message processing.
     */
    static long int WINAPI messageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    /**
     * @brief Updates the message queue for processing.
     *
     * This function checks for and processes any messages in the application's
     * message queue.
     *
     * @return An error code indicating the result of the operation.
     */
    int updateMessageQueue();

    /**
     * @brief Sets the input file name for the model.
     *
     * This function stores the name of the input file to be loaded by the application.
     *
     * @param sFileName The name of the input file.
     */
    void setFileName(const std::string & sFileName) { m_sInputFileName = sFileName; }

    /**
     * @brief Handles a key press event.
     *
     * This function is called when a key is pressed, and it processes the input
     * accordingly.
     *
     * @param iKey The key code of the pressed key.
     */
    void handleKeyPressed(int iKey);

protected:

private:
    /**
     * @brief Default constructor for the application.
     *
     * The constructor is private to ensure the class is used as a singleton.
     */
    CApp() = default;

    /**
     * @brief Loads the 3D model from the specified file.
     *
     * This function loads the model data from a file into the application.
     *
     * @return An error code indicating the result of the operation.
     */
    Err loadFile();

    /**
     * @brief Sets the window focus state.
     *
     * This function sets whether the application window has focus or not.
     *
     * @param bFocus Boolean indicating whether the window has focus.
     */
    void setWindowFocus(bool bFocus) { m_bWindowHasFocus = bFocus; }

    /**
     * @brief Gets the current window focus state.
     *
     * This function returns whether the application window currently has focus.
     *
     * @return Boolean indicating whether the window has focus.
     */
    bool getWindowFocus() const { return m_bWindowHasFocus; }

    /**
     * @brief Handles the event when the left mouse button (LMB) is pressed.
     *
     * This function is called when the user presses or keeps pressing the left mouse button. It processes
     * mouse X and Y position change and rotates the 3D model around Y and X axes accordingly.
     *
     * @param iMouseX The X-coordinate of the mouse position at the time of the event.
     * @param iMouseY The Y-coordinate of the mouse position at the time of the event.
     */
    void handleLMBPressed(int iMouseX, int iMouseY);

    /**
     * @brief Handles the event when the left mouse button (LMB) is released.
     *
     * This function is called when the user releases the left mouse button. It stops model rotation
     * around the Y and X axes.
     */
    void handleLMBReleased();

    /**
     * @brief Handles the event when the middle mouse button (MMB) is pressed.
     *
     * This function is called when the user presses or keeps pressing the middle mouse button. It processes
     * mouse X position change and rotates the 3D model around the Z axis.
     *
     * @param iMouseX The X-coordinate of the mouse position at the time of the event.
     * @param iMouseY The Y-coordinate of the mouse position at the time of the event.
     */
    void handleMMBPressed(int iMouseX, int iMouseY);

    /**
     * @brief Handles the event when the middle mouse button (MMB) is released.
     *
     * This function is called when the user releases the middle mouse button. It stops model rotation
     * around the Z axis.
     */
    void handleMMBReleased();

    /**
     * @brief Handles the event when the right mouse button (RMB) is pressed.
     *
     * This function is called when the user presses or keeps pressing the right mouse button. It processes
     * mouse X and Y position and changes the 3D model display position accordingly.
     *
     * @param iMouseX The X-coordinate of the mouse position at the time of the event.
     * @param iMouseY The Y-coordinate of the mouse position at the time of the event.
     */
    void handleRMBPressed(int iMouseX, int iMouseY);

    /**
     * @brief Handles the event when the right mouse button (RMB) is released.
     *
     * This function is called when the user releases the right mouse button. It stops model position change.
     */
    void handleRMBReleased();

    HWND m_hWindowHandle{nullptr}; ///< Handle to the application window.
    CRenderer m_oRenderer{}; ///< Renderer responsible for displaying the model.
    CModel m_oModel{}; ///< Model representing the 3D object.
    std::string m_sInputFileName{}; ///< The file name of the input model.
    bool m_bWindowHasFocus{false}; ///< Flag indicating if the window has focus.

    // Flags and positions for mouse dragging behavior.
    bool m_bLmbDragging{false}; ///< Flag for left mouse button dragging.
    bool m_bMmbDragging{false}; ///< Flag for middle mouse button dragging.
    bool m_bRmbDragging{false}; ///< Flag for right mouse button dragging.
    int m_iLmbDragMouseStartPosX{0}; ///< Starting position of left mouse button drag (X-axis).
    int m_iLmbDragMouseStartPosY{0}; ///< Starting position of left mouse button drag (Y-axis).
    int m_iMmbDragMouseStartPosX{0}; ///< Starting position of middle mouse button drag (X-axis).
    int m_iMmbDragMouseStartPosY{0}; ///< Starting position of middle mouse button drag (Y-axis).
    int m_iRmbDragMouseStartPosX{0}; ///< Starting position of right mouse button drag (X-axis).
    int m_iRmbDragMouseStartPosY{0}; ///< Starting position of right mouse button drag (Y-axis).

    static constexpr float M_PI{3.14159265358979323846}; ///< Constant for the value of Pi.
};

#endif // STL_VIEWER_CAPP_H_INCLUDED
