/**
 * @file CRenderer.h
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#ifndef STL_VIEWER_CRENDERER_H_INCLUDED
#define STL_VIEWER_CRENDERER_H_INCLUDED
#include "common.h"
#include <windows.h>
#include "CModel.h"
#include "CFpsCounter.h"
#include "CQuaternion.h"

/**
 * @class CRenderer
 * @brief Handles rendering of the 3D model in the OpenGL context.
 *
 * The CRenderer class manages the rendering settings, drawing
 * of both 3D and 2D components, and handles OpenGL context creation.
 */
class CRenderer
{
public:
    /**
     * @enum DrawMode
     * @brief Enum to define different drawing modes for rendering.
     *
     * This enum defines the available rendering modes for the 3D model:
     * - `shading`: Render with shading.
     * - `wireframe`: Render only the wireframe.
     * - `filledWires`: Render a combination of filled polygons and wireframes.
     */
    enum class DrawMode { shading, wireframe, filledWires };

    /**
     * @brief Toggles the animation state.
     *
     * This function switches between playing and pausing the animation.
     */
    void togglePlayAnimation() { m_bAnime = !m_bAnime; }

    /**
     * @brief Gets the window handle of the rendering window.
     *
     * This function returns the handle of the window used for rendering.
     *
     * @return The window handle.
     */
    HWND getWindowHandle() const { return m_hWindowHandle; }

    /**
     * @brief Initializes the renderer and creates the window.
     *
     * This function sets up the rendering context and
     * initializes the necessary OpenGL functionalities
     * for rendering.
     *
     * @param pMsgHandler Pointer to the message handler procedure
     * for the created window.
     *
     * @return An error code indicating the result of the initialization.
     */
    Err init(WNDPROC pMsgHandler);

    /**
     * @brief Destroys the renderer and cleans up resources.
     *
     * This function releases the resources used by the renderer,
     * including the rendering context.
     */
    void destroy();

    /**
     * @brief Sets the viewport and view settings.
     *
     * This function defines the viewport dimensions for rendering based on
     * the width and height provided.
     *
     * @param iWidth The width of the viewport.
     * @param iHeight The height of the viewport.
     */
    void setView(int iWidth, int iHeight);

    /**
     * @brief Draws the scene in the application window.
     *
     * This function renders the entire scene, including both 3D objects
     * and 2D elements that might be overlaid on top of the scene.
     *
     * @param oModel The model to render.
     *
     * @return An error code indicating the result of the rendering operation.
     */
    Err redrawWindow(const CModel &oModel);

    /**
     * @brief Resets the view state of the renderer.
     *
     * This function initializes the view, position, zoom, and frame settings.
     */
    void resetViewState();

    /**
     * @brief Adjusts the zoom level based on mouse scroll activity.
     *
     * This function modifies the zoom variable in the renderer based on the input
     * ratio from user interaction.
     *
     * @param fZoomRatio The ratio of zoom change.
     */
    void zoom(float fZoomRatio);

    /**
     * @brief Toggles between different drawing modes.
     *
     * This function changes the rendering mode for the 3D model, allowing toggling
     * between shading, wireframe, and filled wireframe modes.
     */
    void setNextDrawMode();

    /**
     * @brief Adjusts the number of triangles to skip during rendering.
     *
     * This function sets the mode for skipping certain triangles based on user input,
     * aiding in performance optimization.
     */
    void setNextSkipTrianglesMode();

    /**
     * @brief Rotates the model view orientation around the X-axis.
     *
     * This function applies a rotation specified by the input angle on
     * the X-axis of the model view.
     *
     * @param fAngle The angle to rotate around the X-axis in radians.
     */
    void rotateX(float fAngle);

    /**
     * @brief Rotates the model view orientation around the Y-axis.
     *
     * This function applies a rotation specified by the input angle on
     * the Y-axis of the model view.
     *
     * @param fAngle The angle to rotate around the Y-axis in radians.
     */
    void rotateY(float fAngle);

    /**
     * @brief Rotates the model view orientation around the Z-axis.
     *
     * This function applies a rotation specified by the input angle on
     * the Z-axis of the model view.
     *
     * @param fAngle The angle to rotate around the Z-axis in radians.
     */
    void rotateZ(float fAngle);

    /**
     * @brief Moves the view position.
     *
     * This function modifies the view's position by adding the specified
     * displacement values to the current view coordinates.
     *
     * @param iX The displacement along the X-axis.
     * @param iY The displacement along the Y-axis.
     */
    void moveViewPos(int iX, int iY) {m_iViewPosX += iX; m_iViewPosY += iY; }

protected:

private:
    /**
     * @brief Creates the rendering window.
     *
     * This function creates the window for rendering using the provided message handler.
     * It sets up the window and ensures the rendering context can be properly created.
     *
     * @param pMsgHandler Pointer to the message handler procedure for the created window.
     *
     * @return An error code indicating the result of window creation.
     */
    Err createWindow(WNDPROC pMsgHandler);

    /**
     * @brief Creates the OpenGL rendering context.
     *
     * This function creates and initializes the OpenGL rendering context
     * necessary for rendering the 3D model in the window.
     *
     * @return An error code indicating the result of the OpenGL context creation.
     */
    Err createRenderContext();

    /**
     * @brief Draws the 3D object in the current rendering context.
     *
     * This function handles the actual drawing of the 3D model using OpenGL.
     *
     * @param oModel The model to draw.
     */
    void drawObject(const CModel &oModel);

    /**
     * @brief Draws flat elements and UI text on the screen.
     *
     * This function renders additional information, such as FPS,
     * model name and statistics, onto the screen.
     *
     * @param oModel The model used to extract relevant information.
     */
    void drawFlatElements(const CModel &oModel);

    /**
     * @brief Clears the screen with a specified color.
     *
     * This function clears the color buffer and the depth buffer
     * to prepare for the next frame rendering.
     */
    void clearScreen() const;

    HWND m_hWindowHandle{nullptr}; ///< Window handle for the rendering window.
    HDC m_hDeviceContext{nullptr}; ///< Device context for the rendering window.
    HGLRC m_hRenderContext{nullptr}; ///< OpenGL rendering context.
    int m_iWidth{0}; ///< Width of the rendering window.
    int m_iHeight{0}; ///< Height of the rendering window.
    DrawMode m_drawMode{DrawMode::shading}; ///< Current drawing mode for rendering.
    bool m_bAnime{true}; ///< Flag to indicate if animation is enabled.
    int m_iFrame{0}; ///< Current frame number for animation.
    CFpsCounter m_oFpsCounter{}; ///< FPS counter to measure rendering performance.
    uint16_t m_u16SkipTriangles{0}; ///< Number of triangles to skip during rendering.
    int m_iViewPosX{0}; ///< X position of the view.
    int m_iViewPosY{0}; ///< Y position of the view.
    float m_fZoom{-8.0f}; ///< Zoom level of the view.
    CQuaternion m_oModelViewOrientation{}; ///< Quaternion representing the model's orientation.
};


/**
 * @brief Overloads the stream operator for drawing the DrawMode.
 *
 * This function allows the DrawMode enum to be printed to an output stream.
 *
 * @param stream The output stream to write to.
 * @param o The DrawMode object to output.
 * @return The output stream with DrawMode value.
 */
std::ostream& operator<<(std::ostream& stream, const CRenderer::DrawMode& o);

#endif // STL_VIEWER_CRENDERER_H_INCLUDED
