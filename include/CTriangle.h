/**
 * @file CTriangle.h
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#ifndef STL_VIEWER_CTRIANGLE_H_INCLUDED
#define STL_VIEWER_CTRIANGLE_H_INCLUDED
#include <ostream>

/**
 * @class CTriangle
 * @brief Represents a triangle in 3D space defined by its vertices.
 *
 * The CTriangle class holds the coordinates of the three vertices
 * and allows for output stream operations.
 */
class CTriangle
{
public:
    /**
     * @brief Constructs a triangle with the given vertices.
     *
     * This constructor initializes the coordinates of the three vertices
     * of the triangle in 3D space.
     *
     * @param fX1 The X-coordinate of the first vertex.
     * @param fY1 The Y-coordinate of the first vertex.
     * @param fZ1 The Z-coordinate of the first vertex.
     * @param fX2 The X-coordinate of the second vertex.
     * @param fY2 The Y-coordinate of the second vertex.
     * @param fZ2 The Z-coordinate of the second vertex.
     * @param fX3 The X-coordinate of the third vertex.
     * @param fY3 The Y-coordinate of the third vertex.
     * @param fZ3 The Z-coordinate of the third vertex.
     */
    CTriangle(float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2, float fX3, float fY3, float fZ3)
        : m_fX1{fX1}, m_fY1{fY1}, m_fZ1{fZ1}, m_fX2{fX2}, m_fY2{fY2}, m_fZ2{fZ2}, m_fX3{fX3}, m_fY3{fY3}, m_fZ3{fZ3} {}

    float m_fX1{0.0f}; ///< The X-coordinate of the first vertex.
    float m_fY1{0.0f}; ///< The Y-coordinate of the first vertex.
    float m_fZ1{0.0f}; ///< The Z-coordinate of the first vertex.
    float m_fX2{0.0f}; ///< The X-coordinate of the second vertex.
    float m_fY2{0.0f}; ///< The Y-coordinate of the second vertex.
    float m_fZ2{0.0f}; ///< The Z-coordinate of the second vertex.
    float m_fX3{0.0f}; ///< The X-coordinate of the third vertex.
    float m_fY3{0.0f}; ///< The Y-coordinate of the third vertex.
    float m_fZ3{0.0f}; ///< The Z-coordinate of the third vertex.
};

/**
 * @brief Overloads the output stream operator for the CTriangle class.
 *
 * This function allows for easy output of triangle vertices to
 * output streams.
 *
 * @param stream The output stream to write to.
 * @param o The CTriangle object to output.
 * @return The output stream with the triangle vertices data.
 */
std::ostream& operator<<(std::ostream& stream, const CTriangle& o);


#endif // STL_VIEWER_CTRIANGLE_H_INCLUDED
