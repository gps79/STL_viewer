/**
 * @file CTextOutput.h
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#ifndef STL_VIEWER_TEXT_OUTPUT_H_INCLUDED
#define STL_VIEWER_TEXT_OUTPUT_H_INCLUDED
#include <windows.h>
#include <array>
#include <utility>
#include <string>

/**
 * @class CTextOutput
 * @brief Handles text rendering on the OpenGL window.
 *
 * The CTextOutput class manages the rendering of text in the OpenGL
 * context, allowing for customization of fonts and cursor positioning.
 */
class CTextOutput
{
public:
    /**
     * @brief Deleted default constructor.
     *
     * The default constructor is deleted to ensure that the object
     * cannot be created without specifying the screen width and height.
     */
	CTextOutput() = delete;

    /**
     * @brief Constructs a CTextOutput object with specified screen dimensions.
     *
     * This constructor initializes the screen width and height, and
     * sets the initial font for text rendering.
     *
     * @param iScreenW The screen width in pixels.
     * @param iScreenH The screen height in pixels.
     */
	CTextOutput(int iScreenW, int iScreenH) : m_iScreenW{iScreenW}, m_iScreenH{iScreenH}
	{
	    setFont(m_iCurrentFont);
	}

    /**
     * @brief Sets the font for text output.
     *
     * This function changes the current font used for rendering text
     * to the specified font number from available fonts.
     *
     * @param iFontNum The index of the font to be set.
     */
	void setFont(int iFontNum);

    /**
     * @brief Sets the cursor position for text output.
     *
     * This function sets the position where the next text will be printed.
     * The coordinates are given in screen space, and the cursor is positioned
     * at (iX, iY).
     *
     * @param iX The x-coordinate of the cursor position.
     * @param iY The y-coordinate of the cursor position.
     */
	void setCursorPos(int iX, int iY) { m_iCursorPosX = iX; m_iCursorPosY = iY; }

    /**
     * @brief Sets the spacing between characters.
     *
     * This function adjusts the space between characters when text is rendered.
     * The default value is 0, meaning no additional spacing.
     *
     * @param iSpacing The number of pixels to space between characters.
     */
	void setSpacing(int iSpacing) { m_iSpacing = iSpacing; }

    /**
     * @brief Prints a line of text to the screen.
     *
     * This function renders a line of text at the current cursor position.
     * It outputs each character of the string in sequence, updating the
     * cursor position after each character.
     *
     * @param sStr The string to print.
     *
     * @return The number of characters printed.
     */
	int printLn(const std::string &sStr);

private:
    /**
     * @brief Array of available GLUT fonts.
     *
     * This static array stores pairs of font handles and their corresponding
     * font sizes. The available fonts can be selected using the index in
     * the array.
     */
	static const std::array<std::pair<LPVOID,int>,7> m_aGlutFonts;
    int m_iScreenW; ///< The width of the screen in pixels.
    int m_iScreenH; ///< The height of the screen in pixels.
    int m_iCurrentFont{0}; ///< The index of the current font.
    int m_iCursorPosX{0}; ///< The x-coordinate of the cursor position.
    int m_iCursorPosY{0}; ///< The y-coordinate of the cursor position.
    int m_iSpacing{0}; ///< The spacing between characters when printing text.
};

#endif // STL_VIEWER_TEXT_OUTPUT_H_INCLUDED
