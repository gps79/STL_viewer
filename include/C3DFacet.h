/**
 * @file C3DFacet.h
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#ifndef STL_VIEWER_C3DFACET_H_INCLUDED
#define STL_VIEWER_C3DFACET_H_INCLUDED

#include "CVector3d.h"

/**
 * @class C3DFacet
 * @brief Represents a 3D facet (triangle) in 3D space.
 *
 * The C3DFacet class holds the three vertices (p1, p2, p3) that define a triangle in 3D space.
 * It also contains a normal vector that represents the orientation of the triangle's surface.
 * This normal vector points outward from the surface of the 3D object.
 */
class C3DFacet
{
public:
    /**
     * @brief Coordinates of the first vertex of the triangle.
     *
     * This is a 3D point (CVector3d) that represents one corner of the facet.
     */
    CVector3d p1{0.0f, 0.0f, 0.0f}; ///< p1, p2, p3: 3D-space points creating a triangle.

    /**
     * @brief Coordinates of the second vertex of the triangle.
     *
     * This is a 3D point (CVector3d) that represents another corner of the facet.
     */
    CVector3d p2{0.0f, 0.0f, 0.0f}; ///< p1, p2, p3: 3D-space points creating a triangle.

    /**
     * @brief Coordinates of the third vertex of the triangle.
     *
     * This is a 3D point (CVector3d) that represents the last corner of the facet.
     */
    CVector3d p3{0.0f, 0.0f, 0.0f}; ///< p1, p2, p3: 3D-space points creating a triangle.

    /**
     * @brief Normal vector of the triangle surface.
     *
     * This vector (CVector3d) represents the normal to the triangle formed by p1, p2, and p3.
     * It points outward from the surface of the 3D object.
     */
    CVector3d normal{0.0f, 0.0f, 0.0f}; ///< Normal vector pointing outward from the facet.

protected:

private:

};

#endif // STL_VIEWER_C3DFACET_H_INCLUDED
