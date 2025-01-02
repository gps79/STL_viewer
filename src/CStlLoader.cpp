/**
 * @file CStlLoader.cpp
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#include "CStlLoader.h"
#include "CLogger.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <algorithm>
#include "CTriangle.h"
#include "CVector3d.h"
#include <math.h>
#include <string>
#include <cstring>
#include <fstream>

using namespace std::literals::string_literals;

constexpr int CStlLoader::StlBinaryHeaderSize;
constexpr int CStlLoader::StlBinaryDataStart;

/**
 * Loads ASCII STL file according to the following specification:
 * http://www.fabbers.com/tech/STL_Format
 */
Err CStlLoader::loadFile(const std::string &sFileName, CModel &oModel)
{
    Err retVal{Err::NoError};

    if (fileExists(sFileName))
    {
        readStlFileFormat(sFileName);

        if ((StlFormat::binary == m_fileFormat) || (StlFormat::ascii == m_fileFormat))
        {
            retVal = allocateMemory(oModel);
            if (Err::NoError == retVal)
            {
                switch (m_fileFormat)
                {
                    case StlFormat::binary:
                        retVal = loadBinary(sFileName, oModel);
                        break;

                    case StlFormat::ascii:
                        retVal = loadAscii(sFileName, oModel);
                        break;

                    default:  // the app should never reach this case
                        retVal = Err::InternalLoaderError;
                        break;
                }
            }
        }
        else
        {
            retVal = Err::InvalidStlFile;
        }
    }
    else
    {
        retVal = Err::FileNotFound;
    }

    return retVal;
}

bool CStlLoader::fileExists(const std::string& sFileName) const
{
    std::ifstream file(sFileName);
    return file.good();
}

std::streampos CStlLoader::getFileSize(const std::string &sFileName) const
{
    std::streampos fileSize = 0;
    std::ifstream file(sFileName, std::ios::binary);
    if (file)
    {
        fileSize = file.tellg();
        file.seekg(0, std::ios::end);
        fileSize = file.tellg() - fileSize;
    }
    else
    {
        logPrint(Debug) << "getFileSize - can't open file";
    }
    return fileSize;
}

void CStlLoader::readStlFileFormat(const std::string &sFileName)
{
    m_u32TriangleNumber = 0;
    m_fileFormat = StlFormat::unknown;

    std::streampos fileSize = getFileSize(sFileName);
    logPrint(Debug) << "file \"" <<  sFileName << "\" size: " << fileSize << "B";
    if (fileSize >= 15) // The minimum size of an empty ASCII file is 15 bytes.
    {
        if (isStlFileAsciiFormat(sFileName))
        {
            logPrint(Debug) << "Detected ASCII STL file with " << m_u32TriangleNumber << " triangles inside";
            m_fileFormat = StlFormat::ascii;
        }
        else
        {
            if (fileSize >= StlBinaryDataStart) // the file must be as big as the file header
            {
                if (isStlFileBinaryFormat(sFileName, fileSize))
                {
                    logPrint(Debug) << "Detected Binary STL file with " << m_u32TriangleNumber << " triangles inside";
                    m_fileFormat = StlFormat::binary;
                }
                else
                {
                    logPrint(Debug) << "Neither ASCII nor Binary STL file";
                }
            }
            else
            {
                logPrint(Debug) << "Error: file size < " << StlBinaryDataStart << "B";
            }
        }
    }
    else
    {
        logPrint(Debug) << "Error: file size < 15B";
    }
}

bool CStlLoader::isStlFileAsciiFormat(const std::string &sFileName)
{
    bool bRetVal{false};
    uint32_t u32TriangleNumber{0};

    logPrint(Trace) << "isStlFileAsciiFormat(\"" << sFileName << "\")";
    std::ifstream file{sFileName};
    if (file)
    {
        // Look for text "solid " in first 6 bytes, indicating the possibility that this is an ASCII STL format.
        std::string sLine;
        getline(file, sLine);
        if (file.good())
        {
            strToLower(sLine);
            if (0 == sLine.find("solid "))
            {
                logPrint(Trace) << "File header 'solid' found";
                while (file.good())
                {
                    getline(file, sLine);
                    if (file.good())
                    {
                        strToLower(sLine);
                        if (std::string::npos != sLine.find("endsolid"))
                        {
                            logPrint(Trace) << "File footer 'endsolid' found";
                            logPrint(Trace) << u32TriangleNumber << " triangles detected";
                            bRetVal = true;
                            break;
                        }
                        else
                        {
                            if (std::string::npos != sLine.find("facet normal"))
                            {
                                ++u32TriangleNumber;
                            }
                        }
                    }
                    else
                    {
                        logPrint(Trace) << "File reading error";
                        u32TriangleNumber = 0;
                        break;
                    }
                }
            }
            else
            {
                logPrint(Trace) << "File header 'solid' not found";
            }
        }
        else
        {
            logPrint(Trace) << "File reading error";
        }
    }
    else
    {
        logPrint(Debug) << "Can't open file";
    }

    logPrint(Trace) << "File is " << ((bRetVal)? "" : "not ") << "ASCII file format" << ((bRetVal)? (" containing "s + std::to_string(u32TriangleNumber) + " triangles"): ""s);
    m_u32TriangleNumber = u32TriangleNumber;
    return bRetVal;
}

/**
 * from http://stackoverflow.com/questions/26171521/verifying-that-an-stl-file-is-ascii-or-binary
 * In binary STL file each facet contains:
 *  - Normals: 3 floats (4 bytes each)
 *  - Vertices: 3x floats (4 bytes each, 12 bytes in total)
 *  - AttributeCount: 1 short (2 bytes)
 * Total: 50 bytes per facet
 */
bool CStlLoader::isStlFileBinaryFormat(const std::string &sFileName, std::streampos fileSize)
{
    bool bRetVal{false};
    uint32_t u32TriangleNumber{0};
    constexpr size_t facetSize = 3*sizeof(float) + 3*3*sizeof(float) + sizeof(uint16_t);

    logPrint(Trace) << "isStlFileBinaryFormat(\"" << sFileName << "\"," << fileSize << ")";
    std::ifstream file(sFileName, std::ios::binary | std::ios::in);
    if (file)
    {
        // Header is from bytes 0-79; u32TriangleNumber starts at byte offset 80.
        file.seekg(80);
        if (file.good())
        {
            // Read the number of triangles, uint32_t (4 bytes), little-endian
            uint8_t au8Buffer[4];
            file.read(reinterpret_cast<char*>(au8Buffer), 4);
            if (file.good())
            {
                u32TriangleNumber = (au8Buffer[3] << 24) | (au8Buffer[2] << 16) | (au8Buffer[1] << 8) | au8Buffer[0];
                // Verify that file size equals the sum of header + nTriangles count(4B) + all triangles
                if ((StlBinaryDataStart + (u32TriangleNumber * facetSize)) == fileSize)
                {
                    logPrint(Trace) << "Binary file with " << u32TriangleNumber << " number of triangles detected";
                    bRetVal = true;
                }
                else
                {
                    logPrint(Trace) << "File size doesn't fit " << u32TriangleNumber << " triangles";
                    u32TriangleNumber = 0;
                }
            }
            else
            {
                logPrint(Trace) << "File reading error";
            }
        }
        else
        {
            logPrint(Trace) << "File seek error";
        }
    }
    else
    {
        logPrint(Debug) << "Can't open file";
    }

    logPrint(Trace) << "File is " << ((bRetVal)? "" : "not ") << "Binary file format" << ((bRetVal)? (" containing "s + std::to_string(u32TriangleNumber) + " triangles"s): ""s);
    m_u32TriangleNumber = u32TriangleNumber;
    return bRetVal;
}

Err CStlLoader::allocateMemory(CModel &oModel)
{
    Err retVal{Err::NoError};

    logPrint(Trace) << "Allocating memory for " << m_u32TriangleNumber << " facets";
    if (m_u32TriangleNumber > 0)
    {
        try
        {
           oModel.getFacets().resize(m_u32TriangleNumber);
        }
        catch(...)
        {
            logPrint(Trace) << "Can't allocate memory";
            retVal = Err::MemAlloc;
        }
    }
    else
    {
        logPrint(Trace) << "Not allocating memory - 0 triangles";
    }
    return retVal;
}

Err CStlLoader::loadBinary(const std::string &sFileName, CModel &oModel)
{
    struct StlBinaryFacet
    {
        float normal[3];
        float point1[3];
        float point2[3];
        float point3[3];
        uint16_t attributes;
    };
    constexpr size_t stlBinaryFacetSize = 3*sizeof(float) + 3*3*sizeof(float) + sizeof(uint16_t);

    Err retVal{Err::NoError};

    logPrint(Trace) << "loadBinary(\"" << sFileName << "\")";
    std::vector<C3DFacet> &vFacets = oModel.getFacets();

    if (m_u32TriangleNumber > 0)
    {
        if (vFacets.size() == m_u32TriangleNumber)
        {
            std::ifstream file(sFileName, std::ios::binary);
            if (file)
            {
                char szModelName[StlBinaryHeaderSize+1]; // let's treat file header as a model name and read it
                file.read(szModelName, StlBinaryHeaderSize);
                if (file.good())
                {
                    szModelName[StlBinaryHeaderSize] = '\0';
                    oModel.setModelName(szModelName);

                    file.seekg(4, std::ios::cur); // skip the number of facets as it is already known (4B)
                    if (file.good())
                    {
                        std::streampos readPos = StlBinaryDataStart;
                        StlBinaryFacet record;
                        for (auto &facet: vFacets)
                        {
                            file.read(reinterpret_cast<char*>(&record), stlBinaryFacetSize); // due to the struct padding stlBinaryFacetSize is used instead of sizeof(record)
                            if (file.good())
                            {
                                readPos += stlBinaryFacetSize;
                                if (std::isfinite(record.point1[0]) && std::isfinite(record.point1[1]) && std::isfinite(record.point1[2]) &&
                                    std::isfinite(record.point2[0]) && std::isfinite(record.point2[1]) && std::isfinite(record.point2[2]) &&
                                    std::isfinite(record.point3[0]) && std::isfinite(record.point3[1]) && std::isfinite(record.point3[2]))
                                {
                                    facet.normal.m_fX = record.normal[0]; // normal
                                    facet.normal.m_fY = record.normal[1];
                                    facet.normal.m_fZ = record.normal[2];
                                    facet.p1.m_fX = record.point1[0]; // point 1
                                    facet.p1.m_fY = record.point1[1];
                                    facet.p1.m_fZ = record.point1[2];
                                    facet.p2.m_fX = record.point2[0]; // point 2
                                    facet.p2.m_fY = record.point2[1];
                                    facet.p2.m_fZ = record.point2[2];
                                    facet.p3.m_fX = record.point3[0]; // point 3
                                    facet.p3.m_fY = record.point3[1];
                                    facet.p3.m_fZ = record.point3[2];
                                }
                                else
                                {
                                    // error in triangle definition
                                    logPrint(Trace) << "Data error at " << readPos << "B";
                                    retVal = Err::TriangleDef;
                                    break;
                                }
                            }
                            else
                            {
                                logPrint(Trace) << "Can't read file";
                                retVal = Err::ReadFile;
                                break;
                            }
                        }
                    }
                    else
                    {
                        logPrint(Trace) << "File seek error";
                        retVal = Err::ReadFile2;
                    }
                }
                else
                {
                    logPrint(Trace) << "Can't read file";
                    retVal = Err::ReadFile;
                }
            }
            else
            {
                logPrint(Trace) << "Can't open file";
                retVal = Err::OpenFile;
            }
        }
        else
        {
            logPrint(Trace) << "Memory buffer size (" << vFacets.size() << ") and triangles number(" << m_u32TriangleNumber << ") don't match";
            retVal = Err::InternalLoaderError2;
        }
    }
    else
    {
        logPrint(Debug) << "File contains empty model";
        retVal = Err::EmptyModel;
    }

    if (Err::NoError != retVal)
    {
        logPrint(Trace) << "Deallocating memory";
        vFacets.clear();
        m_u32TriangleNumber = 0;
    }
    return retVal;
}

Err CStlLoader::loadAscii(const std::string &sFileName, CModel &oModel)
{
    Err retVal{Err::NoError};

    logPrint(Trace) << "loadAscii(\"" << sFileName << "\")";
    std::vector<C3DFacet> &vFacets = oModel.getFacets();

    if (m_u32TriangleNumber > 0)
    {
        if (vFacets.size() == m_u32TriangleNumber)
        {
            std::ifstream file(sFileName);
            if (file)
            {
                uint32_t u32CurrentLineNo{0};
                std::string sLine;
                getline(file, sLine);
                ++u32CurrentLineNo;
                if (file.good())
                {
                    strToLower(sLine);
                    if (0 == sLine.find("solid ")) // file starts with "solid"
                    {
                        oModel.setModelName(sLine.substr(sizeof("solid ")-1));
                        for (auto &facet: vFacets)
                        {
                            retVal = stlAsciiReadFacet(file, facet, u32CurrentLineNo);
                            if (Err::NoError != retVal)
                            {
                                break;
                            }
                        }

                        // look for: "endsolid" in file
                        if (Err::NoError == retVal) // only if we exited the "for" loop without error
                        {
                            getline(file, sLine);
                            ++u32CurrentLineNo;
                            if (file.good()) // read last file line
                            {
                                strToLower(sLine);
                                if (0 != sLine.find("endsolid")) // if not found on 0-position
                                {
                                    logPrint(Trace) << "Line:" << u32CurrentLineNo << " 'endsolid' expected";
                                    retVal = Err::StlEndsolid; // "endsolid" expected
                                }
                            }
                            else
                            {
                                logPrint(Trace) << "Line:" << u32CurrentLineNo << " Can't read file";
                                retVal = Err::StlGetline4; // can't read first line of the file
                            }
                        }
                    }
                    else
                    {
                        logPrint(Trace) << "Line:" << u32CurrentLineNo << " 'solid ' expected";
                        retVal = Err::StlSolidExpected; // wrong first line; "solid ....." expected
                    }
                }
                else
                {
                    logPrint(Trace) << "Line:" << u32CurrentLineNo << " Can't read file";
                    retVal = Err::StlGetline5; // can't read first line of the file
                }
            }
            else
            {
                logPrint(Trace) << "Can't open file";
                retVal = Err::OpenFile;
            }
        }
        else
        {
            logPrint(Trace) << "Memory buffer size (" << vFacets.size() << ") and triangles number(" << m_u32TriangleNumber << ") don't match";
            retVal = Err::InternalLoaderError2;
        }
    }
    else
    {
        logPrint(Debug) << "File contains empty model";
        retVal = Err::EmptyModel;
    }

    if (Err::NoError != retVal)
    {
        logPrint(Trace) << "Deallocating memory";
        vFacets.clear();
        m_u32TriangleNumber = 0;
    }
    return retVal;
}

Err CStlLoader::stlAsciiReadLineAndCheck(std::ifstream &file, const std::string &sExpected, uint32_t &u32CurrentLineNo)
{
    Err retVal{Err::NoError};
    std::string sLine;

    getline(file, sLine);
    ++u32CurrentLineNo;
    if (file.good())
    {
        strToLower(sLine);
        size_t pos = sLine.find_first_not_of(" "); // indentation with spaces is allowed
        if (sLine.find(sExpected, pos) != pos)
        {
            logPrint(Error) << "Line:" << u32CurrentLineNo << " '" << sExpected << "' expected";
            retVal = Err::StlAscUnexpected; // "outer loop" expected
        }
    }
    else
    {
        logPrint(Trace) << "Line:" << u32CurrentLineNo << " Can't read file";
        retVal = Err::StlGetline;
    }

    return retVal;
}

Err CStlLoader::stlAsciiReadFacet(std::ifstream &file, C3DFacet &facet, uint32_t &u32CurrentLineNo)
{
    Err retVal{Err::NoError};

    retVal = readAsciiVertex(file, "facet normal ", u32CurrentLineNo, facet.normal); // read expected text: "facet normal ....."
    if (Err::NoError == retVal)
    {
        retVal = stlAsciiReadLineAndCheck(file, "outer loop", u32CurrentLineNo); // read expected text: "outer loop"
        if (Err::NoError == retVal)
        {
            retVal = readAsciiVertex(file, "vertex ", u32CurrentLineNo, facet.p1); // read "vertex ...."
            if (Err::NoError == retVal)
            {
                retVal = readAsciiVertex(file, "vertex ", u32CurrentLineNo, facet.p2); // read "vertex ...."
                if (Err::NoError == retVal)
                {
                    retVal = readAsciiVertex(file, "vertex ", u32CurrentLineNo, facet.p3); // read "vertex ...."
                    if (Err::NoError == retVal)
                    {
                        retVal = stlAsciiReadLineAndCheck(file, "endloop", u32CurrentLineNo); // read expected text: "endloop"
                    }
                }
            }
        }
        if (Err::NoError == retVal)
        {
            retVal = stlAsciiReadLineAndCheck(file, "endfacet", u32CurrentLineNo); // look for: "endfacet" in file
        }
    }
    return retVal;
}

Err CStlLoader::stringToFloat(const std::string &sStr, float &fNumber) const
{
    Err retVal{Err::NoError};

    try
    {
        fNumber = std::stof(sStr);
    }
    catch (...)
    {
        logPrint(Debug) << "stringToFloat:" << sStr;
        retVal = Err::StlConvertToFloat;
    }

    return retVal;
}

Err CStlLoader::readAsciiVertex(std::ifstream &file, const std::string &sHeader, size_t &u32CurrentLineNo, CVector3d &oVertex)
{
    // converts a text line of format: "<header> <float> <float> <float>" to a 3D vertex
    Err retVal{Err::NoError};

    std::string sLine;
    getline(file, sLine);
    ++u32CurrentLineNo;
    if (file.good())
    {
        strToLower(sLine);
        size_t pos = sLine.find_first_not_of(" ");
        if (sLine.find(sHeader, pos) == pos)
        {
            size_t number1Begin = sLine.find_first_not_of(" ", pos+sHeader.length()); // beginning of the first <float>
            if (std::string::npos != number1Begin)
            {
                size_t number1End = sLine.find(" ", number1Begin); // end of the first <float>
                if (std::string::npos != number1End)
                {
                    --number1End;
                    size_t number2Begin = sLine.find_first_not_of(" ", number1End+1); // beginning of the second <float>
                    if (std::string::npos != number2Begin)
                    {
                        size_t number2End = sLine.find(" ", number2Begin);// end of the second <float>
                        if (std::string::npos != number2End)
                        {
                            --number2End;
                            size_t number3Begin = sLine.find_first_not_of(" ", number2End+1); // beginning of the third <float>
                            if (std::string::npos != number3Begin)
                            {
                                size_t number3End = sLine.length()-1;// end of the third <float>

                                // now convert strings to floats
                                retVal = stringToFloat(sLine.substr(number1Begin, number1End-number1Begin+1), oVertex.m_fX);
                                if (Err::NoError == retVal)
                                {
                                    retVal = stringToFloat(sLine.substr(number2Begin, number2End-number2Begin+1), oVertex.m_fY);
                                    if (Err::NoError == retVal)
                                    {
                                        retVal = stringToFloat(sLine.substr(number3Begin, number3End-number3Begin+1), oVertex.m_fZ);
                                        if (Err::NoError != retVal)
                                        {
                                            logPrint(Trace) << "Line:" << u32CurrentLineNo << " third argument after '" << sHeader << "' conversion to 'float' error";
                                        }
                                    }
                                    else
                                    {
                                        logPrint(Trace) << "Line:" << u32CurrentLineNo << " second argument after '" << sHeader << "' conversion to 'float' error";
                                    }
                                }
                                else
                                {
                                    logPrint(Trace) << "Line:" << u32CurrentLineNo << " first argument after '" << sHeader << "' conversion to 'float' error";
                                }
                            }
                            else
                            {
                                logPrint(Trace) << "Line:" << u32CurrentLineNo << " third argument after '" << sHeader << "' expected";
                                retVal = Err::StlVertFindNum3Beg;
                            }
                        }
                        else
                        {
                            logPrint(Trace) << "Line:" << u32CurrentLineNo << " third argument after '" << sHeader << "' expected";
                            retVal = Err::StlVertFindNum2End;
                        }
                    }
                    else
                    {
                        logPrint(Trace) << "Line:" << u32CurrentLineNo << " second argument after '" << sHeader << "' expected";
                        retVal = Err::StlVertFindNum2Beg;
                    }
                }
                else
                {
                    logPrint(Trace) << "Line:" << u32CurrentLineNo << " second argument after '" << sHeader << "' expected";
                    retVal = Err::StlVertFindNum1End;
                }
            }
            else
            {
                logPrint(Trace) << "Line:" << u32CurrentLineNo << " first argument after '" << sHeader << "' expected";
                retVal = Err::StlVertFindNum1Beg;
            }
        }
        else
        {
            logPrint(Trace) << "Line:" << u32CurrentLineNo << " '" << sHeader << "' expected";
            retVal = Err::StlVertFindSpace;
        }
    }
    else
    {
        logPrint(Trace) << "Line:" << u32CurrentLineNo << " Can't read file";
        retVal = Err::StlVertGetline;
    }
    return retVal;
}


