/**
 * @file CStlLoader.h
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#ifndef STL_VIEWER_CSTLLOADER_H_INCLUDED
#define STL_VIEWER_CSTLLOADER_H_INCLUDED

#include<stdint.h>
#include<string>
#include<vector>
#include<utility>
#include "common.h"
#include "C3DFacet.h"
#include "CModel.h"

/**
 * @class CStlLoader
 * @brief Responsible for loading STL files and populating the model.
 *
 * The CStlLoader class provides methods to read both ASCII and binary
 * STL files and allocate corresponding memory for the model representation.
 */
class CStlLoader
{
public:
    /**
     * @enum StlFormat
     * @brief Enum to define the format types of the STL file.
     *
     * This enum specifies the possible formats of the STL file:
     * - `notChecked`: Format has not been determined.
     * - `binary`: The STL file is in binary format.
     * - `ascii`: The STL file is in ASCII format.
     * - `unknown`: The STL file format is unknown or invalid.
     */
    enum class StlFormat { notChecked, binary, ascii, unknown };

    /**
     * @brief Loads a 3D model from a specified STL file.
     *
     * This function reads the content of an STL file (either binary or ASCII),
     * and populates the provided model object with the data.
     *
     * @param sFileName The name of the STL file to load.
     * @param oModel The model object to populate with the loaded data.
     *
     * @return An error code indicating the result of the operation.
     */
    Err loadFile(const std::string &sFileName, CModel &oModel);

    /**
     * @brief Gets the file type of the loaded STL file.
     *
     * This function returns the format of the STL file (binary, ASCII, or unknown).
     *
     * @return The format of the STL file.
     */
    StlFormat getFileType() const { return m_fileFormat; }

protected:

private:
    /**
     * @brief Checks if a given file exists.
     *
     * This function checks the existence of a specified file by
     * attempting to open it for reading.
     *
     * @param sFileName The name of the file to check.
     *
     * @return True if the file exists; otherwise false.
     */
    bool fileExists(const std::string& sFileName) const;

    /**
     * @brief Retrieves the size of the specified file in bytes.
     *
     * This function opens the file and determines its size, which allows
     * for proper validation of STL file data.
     *
     * @param sFileName The name of the file whose size to retrieve.
     *
     * @return The size of the file in bytes.
     */
    std::streampos getFileSize(const std::string &sFileName) const;

    /**
     * @brief Reads the format of the specified STL file.
     *
     * This function examines the file to determine whether it is in
     * ASCII or binary format.
     *
     * @param sFileName The name of the file whose format to read.
     */
    void readStlFileFormat(const std::string &sFileName);

    /**
     * @brief Checks if the given STL file is in ASCII format.
     *
     * This function determines if the file is ASCII format by searching
     * for the presence of the "solid " keyword in the file.
     *
     * @param sFileName The name of the STL file to check.
     *
     * @return True if the file is in ASCII format; false otherwise.
     */
    bool isStlFileAsciiFormat(const std::string &sFileName);

    /**
     * @brief Checks if the given STL file is in binary format.
     *
     * This function checks if the file is a valid binary STL file by
     * reading its header and verifying its content.
     *
     * @param sFileName The name of the STL file to check.
     * @param fileSize The total size of the file in bytes.
     *
     * @return True if the file is in binary format; false otherwise.
     */
    bool isStlFileBinaryFormat(const std::string &sFileName, std::streampos fileSize);

    /**
     * @brief Allocates memory for the model based on the triangle count.
     *
     * This function attempts to resize the facets vector based on the
     * number of triangles read from the STL file.
     *
     * @param oModel The model object for which memory allocation occurs.
     *
     * @return An error code indicating the result of the memory allocation.
     */
    Err allocateMemory(CModel &oModel);

    /**
     * @brief Loads a binary STL file.
     *
     * This function reads a binary STL file and populates the model
     * data structure with its contents.
     *
     * @param sFileName The name of the STL file to load.
     * @param oModel The model object to populate with the loaded data.
     *
     * @return An error code indicating the result of the operation.
     */
    Err loadBinary(const std::string &sFileName, CModel &oModel);

    /**
     * @brief Loads an ASCII STL file.
     *
     * This function reads an ASCII STL file line by line, extracting
     * the triangle data and populating the model data structure.
     *
     * @param sFileName The name of the STL file to load.
     * @param oModel The model object to populate with the loaded data.
     *
     * @return An error code indicating the result of the operation.
     */
    Err loadAscii(const std::string &sFileName, CModel &oModel);

    /**
     * @brief Reads a line from the ASCII STL file and checks for expected content.
     *
     * This function reads lines from the file while checking if the content matches
     * the expected header.
     *
     * @param file The input file stream.
     * @param sExpected The header string that we expect to find.
     * @param u32CurrentLineNo The current line number in the file.
     *
     * @return An error code indicating the result of the read operation.
     */
    Err stlAsciiReadLineAndCheck(std::ifstream &file, const std::string &sExpected, uint32_t &u32CurrentLineNo);

    /**
     * @brief Reads the triangle data from a facet in the ASCII STL file.
     *
     * This function processes a facet from the STL file, extracting the
     * normal and vertices, and checking for correct formatting.
     *
     * @param file The input file stream.
     * @param facet The facet to populate with the read information.
     * @param u32CurrentLineNo The current line number in the file.
     *
     * @return An error code indicating the result of the read operation.
     */
    Err stlAsciiReadFacet(std::ifstream &file, C3DFacet &facet, uint32_t &u32CurrentLineNo);

    /**
     * @brief Reads a vertex from an ASCII STL file.
     *
     * This function extracts a vertex from the STL file line based on
     * the specified header.
     *
     * @param file The input file stream.
     * @param sHeader The header to search for ("facet normal", "vertex", etc.).
     * @param u32CurrentLineNo The current line number.
     * @param oVertex The vertex object to populate.
     *
     * @return An error code indicating the result of the read operation.
     */
    Err readAsciiVertex(std::ifstream &file, const std::string &sHeader, size_t &u32CurrentLineNo, CVector3d &oVertex);

    /**
     * @brief Converts a string to a floating-point number.
     *
     * This function attempts to convert a string representation of a
     * number into its float representation.
     *
     * @param sStr The string to convert.
     * @param fNumber The resulting float number.
     *
     * @return An error code indicating the result of the conversion.
     */
    Err stringToFloat(const std::string &sStr, float &fNumber) const;

    static constexpr int StlBinaryHeaderSize = 80; ///< Size of the STL binary header.
    static constexpr int StlBinaryDataStart = 84; ///< Start position of the binary data in the STL file.

    StlFormat m_fileFormat{StlFormat::notChecked}; ///< The format of the STL file.
    uint32_t m_u32TriangleNumber{0}; ///< Number of triangles in the STL file.
};

#endif // STL_VIEWER_CSTLLOADER_H_INCLUDED
