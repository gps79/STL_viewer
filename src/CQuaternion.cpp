/**
 * @file CQuaternion.cpp
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#include "CQuaternion.h"
#include <cmath>

/* Quaternions theory
 * @see http://warsztat.gd/wiki/Kwaterniony+w+praktyce
 * @see https://community.khronos.org/t/problem-with-rotation-along-rotated-normals/49870/19
 *
 * multQuat multiplies another quaternion onto it. If you want to rotate your object phi radians around the axis (x,y,z), you have to call:
 * multQuat(x*sin(phi/2), y*sin(phi/2), z*sin(phi/2), cos(phi/2));
 *
 * The transformation code should look like this:
 * multiple(sin(.005), 0, 0, cos(.005)); - rotation around X-axis
 * multiple(0, sin(.005), 0, cos(.005)); - rotation around Y-axis
 * multiple(0, 0, sin(.005), cos(.005)); - rotation around Z-axis
 * normalize();
 * rot = toMatrix();
 * glMultMatrixf(rot);
 */
void CQuaternion::reset()
{
    m_fX = 0.0f;
    m_fY = 0.0f;
    m_fZ = 0.0f;
    m_fW = 1.0f;
}

void CQuaternion::normalize()
{
    float fLength = std::sqrt(m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ + m_fW * m_fW);
    m_fX /= fLength;
    m_fY /= fLength;
    m_fZ /= fLength;
    m_fW /= fLength;
}

void CQuaternion::multiple(float fX, float fY, float fZ, float fW)
{
    float fNewX = fX * m_fW + fW * m_fX + fZ * m_fY - fY * m_fZ;
    float fNewY = fY * m_fW - fZ * m_fX + fW * m_fY + fX * m_fZ;
    float fNewZ = fZ * m_fW + fY * m_fX - fX * m_fY + fW * m_fZ;
    float fNewW = fW * m_fW - fX * m_fX - fY * m_fY - fZ * m_fZ;
    m_fW = fNewW;
    m_fX = fNewX;
    m_fY = fNewY;
    m_fZ = fNewZ;
}

TQuatMatrix CQuaternion::toMatrix()
{
    normalize();

    float fXX = m_fX * m_fX;
    float fXY = m_fX * m_fY;
    float fXZ = m_fX * m_fZ;
    float fXW = m_fX * m_fW;
    float fYY = m_fY * m_fY;
    float fYZ = m_fY * m_fZ;
    float fYW = m_fY * m_fW;
    float fZZ = m_fZ * m_fZ;
    float fZW = m_fZ * m_fW;

    TQuatMatrix fMatrix;
    fMatrix[0] = 1.0f - 2.0f * ( fYY + fZZ );
    fMatrix[1] = 2.0f * ( fXY - fZW );
    fMatrix[2] = 2.0f * ( fXZ + fYW );
    fMatrix[4] = 2.0f * ( fXY + fZW );
    fMatrix[5] = 1.0f - 2.0f * ( fXX + fZZ );
    fMatrix[6] = 2.0f * ( fYZ - fXW );
    fMatrix[8] = 2.0f * ( fXZ - fYW );
    fMatrix[9] = 2.0f * ( fYZ + fXW );
    fMatrix[10] = 1.0f - 2.0f * ( fXX + fYY );
    fMatrix[3] = fMatrix[7] = fMatrix[11] = fMatrix[12] = fMatrix[13] = fMatrix[14] = 0.0f;
    fMatrix[15] = 1.0f;

    return fMatrix;
}

