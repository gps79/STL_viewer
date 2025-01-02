/**
 * @file CVector3d.h
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#ifndef STL_VIEWER_CVECTOR3D_H_INCLUDED
#define STL_VIEWER_CVECTOR3D_H_INCLUDED
#include <math.h>
#include <ostream>

/**
 * @class CVector3d
 * @brief Represents a 3D vector in space.
 *
 * The CVector3d class holds the coordinates of a vector in 3D space,
 * allowing for vector operations and output.
 */
class CVector3d
{
public:
    /**
     * @brief Constructs a 3D vector with the given coordinates.
     *
     * This constructor initializes the X, Y, and Z coordinates of the vector.
     *
     * @param fX The X-coordinate of the vector.
     * @param fY The Y-coordinate of the vector.
     * @param fZ The Z-coordinate of the vector.
     */
    CVector3d(float fX, float fY, float fZ) : m_fX{fX}, m_fY{fY}, m_fZ{fZ} {}

    float m_fX{0.0f}; ///< The X-coordinate of the vector.
    float m_fY{0.0f}; ///< The Y-coordinate of the vector.
    float m_fZ{0.0f}; ///< The Z-coordinate of the vector.

};

/**
 * @brief Overloads the output stream operator for the CVector3d class.
 *
 * This function allows for easy output of 3D vector components to
 * output streams.
 *
 * @param stream The output stream to write to.
 * @param o The CVector3d object to output.
 * @return The output stream with the vector components data.
 */
std::ostream& operator<<(std::ostream& stream, const CVector3d& o);


#endif // STL_VIEWER_CVECTOR3D_H_INCLUDED
