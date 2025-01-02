/**
 * @file CTriangle.cpp
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#include "CTriangle.h"

std::ostream& operator<<(std::ostream& stream, const CTriangle& o)
{
	stream << "<" << o.m_fX1 << "," << o.m_fY1 << "," << o.m_fZ1 << ">,<";
	stream << o.m_fX2 << "," << o.m_fY2 << "," << o.m_fZ2 << ">,<";
	stream << o.m_fX3 << "," << o.m_fY3 << "," << o.m_fZ3 << ">";
	return stream;
}
