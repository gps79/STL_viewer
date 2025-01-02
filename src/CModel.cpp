/**
 * @file CModel.cpp
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#include <windows.h>
#include "CModel.h"
#include "CLogger.h"
#include "CStlLoader.h"
#include "CTriangle.h"
#include "CVector3d.h"
#include <sstream>
#include <iomanip>
#include <GL/freeglut.h>
#include <map>
#include <string>
#include <iostream>

using namespace std::literals::string_literals;


void CModel::normalizeModel()
{
    logPrint(Debug) << "normalizeModel";
    // normalize and center the model
    if (m_vFacets.size() > 0)
    {
        // find Min and Max values in all dimensions
        float fMinX = m_vFacets[0].p1.m_fX;
        float fMaxX = m_vFacets[0].p1.m_fX;
        float fMinY = m_vFacets[0].p1.m_fY;
        float fMaxY = m_vFacets[0].p1.m_fY;
        float fMinZ = m_vFacets[0].p1.m_fZ;
        float fMaxZ = m_vFacets[0].p1.m_fZ;
        for (const auto &oFacet : m_vFacets)
        {
            fMinX = std::min({fMinX, oFacet.p1.m_fX, oFacet.p2.m_fX, oFacet.p3.m_fX});
            fMaxX = std::max({fMaxX, oFacet.p1.m_fX, oFacet.p2.m_fX, oFacet.p3.m_fX});
            fMinY = std::min({fMinY, oFacet.p1.m_fY, oFacet.p2.m_fY, oFacet.p3.m_fY});
            fMaxY = std::max({fMaxY, oFacet.p1.m_fY, oFacet.p2.m_fY, oFacet.p3.m_fY});
            fMinZ = std::min({fMinZ, oFacet.p1.m_fZ, oFacet.p2.m_fZ, oFacet.p3.m_fZ});
            fMaxZ = std::max({fMaxZ, oFacet.p1.m_fZ, oFacet.p2.m_fZ, oFacet.p3.m_fZ});
        }
        float fScale = std::max({fMaxX-fMinX, fMaxY-fMinY, fMaxZ-fMinZ});
        if (fScale > 0.0f)
        {
            fScale = 1.0f / fScale;
        }
        else
        {
            fScale = 0.0f;
        }
        float fShiftX = fMinX + 0.5f*(fMaxX-fMinX);
        float fShiftY = fMinY + 0.5f*(fMaxY-fMinY);
        float fShiftZ = fMinZ + 0.5f*(fMaxZ-fMinZ);

        logPrint(Debug) << "Normalizing model:";
        logPrint(Debug) << "scale=" << fScale;
        logPrint(Debug) << "shiftx=" << fShiftX;
        logPrint(Debug) << "shifty=" << fShiftY;
        logPrint(Debug) << "shiftz=" << fShiftZ;

        for (auto &oFacet : m_vFacets)
        {
            oFacet.p1.m_fX = (oFacet.p1.m_fX - fShiftX) * fScale;
            oFacet.p1.m_fY = (oFacet.p1.m_fY - fShiftY) * fScale;
            oFacet.p1.m_fZ = (oFacet.p1.m_fZ - fShiftZ) * fScale;

            oFacet.p2.m_fX = (oFacet.p2.m_fX - fShiftX) * fScale;
            oFacet.p2.m_fY = (oFacet.p2.m_fY - fShiftY) * fScale;
            oFacet.p2.m_fZ = (oFacet.p2.m_fZ - fShiftZ) * fScale;

            oFacet.p3.m_fX = (oFacet.p3.m_fX - fShiftX) * fScale;
            oFacet.p3.m_fY = (oFacet.p3.m_fY - fShiftY) * fScale;
            oFacet.p3.m_fZ = (oFacet.p3.m_fZ - fShiftZ) * fScale;
        }
    }
}

void CModel::rotateX() // left-hand rotation by 90 degrees around X-axis
{
    // 90-degree rotation algorithm:
    // new x <- old x
    // new y <- old z
    // new z <- old -y
    logPrint(Debug) << "Model - rotateX";
    for (auto &oFacet : m_vFacets)
    {
        float fTemp;
        fTemp = oFacet.p1.m_fY;
        oFacet.p1.m_fY = oFacet.p1.m_fZ;
        oFacet.p1.m_fZ = -fTemp;

        fTemp = oFacet.p2.m_fY;
        oFacet.p2.m_fY = oFacet.p2.m_fZ;
        oFacet.p2.m_fZ = -fTemp;

        fTemp = oFacet.p3.m_fY;
        oFacet.p3.m_fY = oFacet.p3.m_fZ;
        oFacet.p3.m_fZ = -fTemp;

        fTemp = oFacet.normal.m_fY;
        oFacet.normal.m_fY = oFacet.normal.m_fZ;
        oFacet.normal.m_fZ = -fTemp;
    }
}

void CModel::rotateY() // left-hand rotation by 90 degrees around Y-axis
{
    // 90-degree rotation algorithm:
    // new x <- old -z
    // new y <- old y
    // new z <- old x
    logPrint(Debug) << "Model - rotateY";
    for (auto &oFacet : m_vFacets)
    {
        float fTemp;
        fTemp = oFacet.p1.m_fZ;
        oFacet.p1.m_fZ = oFacet.p1.m_fX;
        oFacet.p1.m_fX = -fTemp;

        fTemp = oFacet.p2.m_fZ;
        oFacet.p2.m_fZ = oFacet.p2.m_fX;
        oFacet.p2.m_fX = -fTemp;

        fTemp = oFacet.p3.m_fZ;
        oFacet.p3.m_fZ = oFacet.p3.m_fX;
        oFacet.p3.m_fX = -fTemp;

        fTemp = oFacet.normal.m_fZ;
        oFacet.normal.m_fZ = oFacet.normal.m_fX;
        oFacet.normal.m_fX = -fTemp;
    }
}

void CModel::rotateZ() // left-hand rotation by 90 degrees around Z-axis
{
    // 90-degree rotation algorithm:
    // new x <- old y
    // new y <- old -x
    // new z <- old z
    logPrint(Debug) << "Model - rotateZ";
    for (auto &oFacet : m_vFacets)
    {
        float fTemp;
        fTemp = oFacet.p1.m_fX;
        oFacet.p1.m_fX = oFacet.p1.m_fY;
        oFacet.p1.m_fY = -fTemp;

        fTemp = oFacet.p2.m_fX;
        oFacet.p2.m_fX = oFacet.p2.m_fY;
        oFacet.p2.m_fY = -fTemp;

        fTemp = oFacet.p3.m_fX;
        oFacet.p3.m_fX = oFacet.p3.m_fY;
        oFacet.p3.m_fY = -fTemp;

        fTemp = oFacet.normal.m_fX;
        oFacet.normal.m_fX = oFacet.normal.m_fY;
        oFacet.normal.m_fY = -fTemp;
    }
}
