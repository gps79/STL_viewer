/**
 * @file CVector3d.cpp
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#include "CVector3d.h"

std::ostream& operator<<(std::ostream& stream, const CVector3d& o)
{
	stream << "<" << o.m_fX << "," << o.m_fY << "," << o.m_fZ << ">";
	return stream;
}


