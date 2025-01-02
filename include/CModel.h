/**
 * @file CModel.h
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#ifndef STL_VIEWER_CMODEL_H_INCLUDED
#define STL_VIEWER_CMODEL_H_INCLUDED
#include <string>
#include "common.h"
#include <vector>
#include "C3DFacet.h"
#include <string>
#include "CVector3d.h"

 /**
 * @class CModel
 * @brief Represents a 3D model composed of facets.
 *
 * The CModel class manages the facets that make up the 3D model. It provides methods for
 * manipulating the model's geometry, including normalizing its coordinates and applying
 * rotations around the X, Y, and Z axes. The model is represented as a collection of 3D facets.
 */
class CModel
{
public:
    /**
     * @brief Gets the list of facets in the model.
     *
     * This function provides access to the vector of facets that make up the model.
     * It can be used to modify the facets or retrieve their information.
     *
     * @return A reference to the vector of facets.
     */
    std::vector<C3DFacet> &getFacets() { return m_vFacets; }

    /**
     * @brief Gets the list of facets in the model (const version).
     *
     * This function provides constant access to the vector of facets that make up the model.
     * It allows for inspecting the facets without modifying them.
     *
     * @return A const reference to the vector of facets.
     */
    const std::vector<C3DFacet> &getFacets() const { return m_vFacets; }

    /**
     * @brief Sets the name of the model.
     *
     * This function allows setting a custom name for the 3D model.
     *
     * @param sName The name to assign to the model.
     */
    void setModelName(const std::string &sName) { m_sName = sName; }

    /**
     * @brief Gets the name of the model.
     *
     * This function returns the name of the model.
     *
     * @return The name of the model.
     */
    const std::string &getModelName() const { return m_sName; }

    /**
     * @brief Normalizes the model coordinates.
     *
     * This function adjusts the vertex coordinates of the model to
     * ensure it is centered and appropriately scaled within the unit cube.
     */
    void normalizeModel();

    /**
     * @brief Rotates the model around the X-axis.
     *
     * This function applies a left-hand rotation of 90 degrees around
     * the X-axis to the model.
     */
    void rotateX();

    /**
     * @brief Rotates the model around the Y-axis.
     *
     * This function applies a left-hand rotation of 90 degrees around
     * the Y-axis to the model.
     */
    void rotateY();

    /**
     * @brief Rotates the model around the Z-axis.
     *
     * This function applies a left-hand rotation of 90 degrees around
     * the Z-axis to the model.
     */
    void rotateZ();

private:
    std::vector<C3DFacet> m_vFacets{}; ///< A vector of facets that constitute the 3D model.
    std::string m_sName{}; ///< The name of the 3D model.
};

#endif // STL_VIEWER_CMODEL_H_INCLUDED
