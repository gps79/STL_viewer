/**
 * @file CQuaternion.h
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#ifndef STL_VIEWER_CQUATERNION_H_INCLUDED
#define STL_VIEWER_CQUATERNION_H_INCLUDED
#include<array>

/**
 * @typedef TQuatMatrix
 * @brief Defines a 4x4 matrix used by OpenGL.
 */
typedef std::array<float, 16> TQuatMatrix;

/**
 * @class CQuaternion
 * @brief Represents a quaternion for 3D rotations.
 *
 * The CQuaternion class provides methods for quaternion operations
 * including normalization and conversion to a rotation matrix.
 */
class CQuaternion
{
public:
    /**
     * @brief Resets the quaternion to identity.
     *
     * This function initializes the quaternion values for
     * identity rotation.
     */
    void reset();

    /**
     * @brief Normalizes the quaternion.
     *
     * This function normalizes the quaternion to ensure that it
     * maintains a unit length, typically needed for rotation calculations.
     */
    void normalize();

    /**
     * @brief Multiplies another quaternion onto this one.
     *
     * This function applies quaternion multiplication, which allows
     * for composite rotations by applying transformations specified
     * in quaternion form.
     *
     * @param fX X component of the quaternion.
     * @param fY Y component of the quaternion.
     * @param fZ Z component of the quaternion.
     * @param fW W component of the quaternion.
     */
    void multiple(float fX, float fY, float fZ, float fW);

    /**
     * @brief Converts the quaternion to a 4x4 matrix.
     *
     * This function computes the corresponding transformation matrix
     * from the quaternion representation, suitable for OpenGL transformations.
     * The function normalizes the quaternion before conversion.
     *
     * @return The resulting 4x4 transformation matrix.
     */
    TQuatMatrix toMatrix();

protected:

private:
    float m_fX{0.0f}; ///< The X component of the quaternion.
    float m_fY{0.0f}; ///< The Y component of the quaternion.
    float m_fZ{0.0f}; ///< The Z component of the quaternion.
    float m_fW{1.0f}; ///< The W (scalar) component of the quaternion, initialized to 1.
};

#endif // STL_VIEWER_CQUATERNION_H_INCLUDED
