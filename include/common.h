/**
 * @file common.h
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#ifndef STL_VIEWER_COMMON_H_INCLUDED
#define STL_VIEWER_COMMON_H_INCLUDED

#include <algorithm>
#include <cctype>

/**
 * @enum Err
 * @brief Defines error codes for the application.
 */
enum class Err : int
{
    NoError = 0,
    GlInitError,
    GlError,
    CantRefgisterWindowClass,
    CantGetAppInstance,
    CantCreateWindow,
    DeviceContext,
    CantFindPixelFormat,
    CantSetPixelFormat,
    CantCreateRenderContext,
    CantBindRenderContext,
    MissingArg,
    CmdLineParse,
    InvalidStlFile,
    FileNotFound,
    InternalLoaderError,
    MemAlloc,
    TriangleDef,
    ReadFile,
    ReadFile2,
    OpenFile,
    InternalLoaderError2,
    EmptyModel,
    StlGetline,
    StlEndsolid,
    StlGetline4,
    StlGetline5,
    StlSolidExpected,
    StlAscUnexpected,
    StlVertGetline,
    StlVertFindSpace,
    StlVertFindNum1Beg,
    StlVertFindNum1End,
    StlVertFindNum2Beg,
    StlVertFindNum2End,
    StlVertFindNum3Beg,
    StlConvertToFloat
};


/**
 * @brief Converts a container of characters to lowercase.
 *
 * This function modifies the input string by converting all its characters
 * to lowercase.
 *
 * @param t The string or container to be converted.
 */
template <typename T>
void strToLower(T &t)
{
    std::for_each(t.begin(), t.end(), [&](auto &a){ a = std::tolower(a); });
}

#endif // STL_VIEWER_COMMON_H_INCLUDED
