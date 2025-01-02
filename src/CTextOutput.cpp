/**
 * @file CTextOutput.cpp
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#include "CTextOutput.h"
#include <GL/freeglut.h>

const std::array<std::pair<LPVOID,int>,7> CTextOutput::m_aGlutFonts
{{
    {GLUT_BITMAP_9_BY_15, 15},
    {GLUT_BITMAP_8_BY_13, 13},
    {GLUT_BITMAP_TIMES_ROMAN_10, 10},
    {GLUT_BITMAP_TIMES_ROMAN_24, 24},
    {GLUT_BITMAP_HELVETICA_10, 10},
    {GLUT_BITMAP_HELVETICA_12, 12},
    {GLUT_BITMAP_HELVETICA_18, 18}
}};

void CTextOutput::setFont(int iFontNum)
{
    if ((iFontNum >= 0) && (iFontNum < static_cast<int>(m_aGlutFonts.size())))
    {
        m_iCurrentFont = iFontNum;
    }
}

int CTextOutput::printLn(const std::string &sStr)
{
	float x1 = m_iCursorPosX;
	auto pFont = m_aGlutFonts[m_iCurrentFont].first;
	auto iFontHeight = m_aGlutFonts[m_iCurrentFont].second;
	for (const char &cChar : sStr)
    {
		glRasterPos2f(x1, m_iCursorPosY + iFontHeight);
		glutBitmapCharacter(pFont, cChar);
		x1 += glutBitmapWidth(pFont, cChar) + m_iSpacing;
    }
	m_iCursorPosY += iFontHeight;
	return sStr.length();
}

