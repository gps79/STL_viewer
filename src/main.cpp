/**
 * @file main.cpp
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 *
 * @mainpage STL viewer application
 * The STL viewer is an application which loads the 3D model from the STL file (both ASCII and binary formats are supported) and displays it in the window.
 * The toolchain:
 * - IDE: Code::Blocks 20.03
 * - Compiler: MinGW GCC-6.3.0-1
 * - OpenGL library: freeGlut-MinGW-3.0.0-1 @see https://www.transmissionzero.co.uk/files/software/development/GLUT/
 *
 * The source code is C++14 compatible, and uses Win32 API and OpenGL freeGlut library.
 *
 * Build the application with -DDEBUG and -DLOGGING_ENABLED switches to enable activity logging to both the "output.log" file and on the console.
 *
 * Recommended (Debug) build switches: -pedantic -Wall -std=c++14 -m32 -g -DDEBUG -DLOGGING_ENABLED
 *
 * Recommended link switches: -static-libstdc++ -static -m32 -static-libgcc -ggdb  -lopengl32 -lglu32 -lgdi32 -lfreeglut
 *
 * @see WinMain application entry point
 */

#include "common.h"
#include "CLogger.h"
#include "CApp.h"

/**
 * @brief Entry point of the application.
 *
 * This function serves as the main entry point of the application
 * initializing logging, initializing the application instance,
 * processing arguments, and executing the application loop.
 *
 * @return Exit code of the application.
 */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Err retVal{Err::NoError};

    CLogger::enableEchoToCout();          // print all logs also on the STDOUT
    logPrint(Info) << "Application start";

    CApp &oApp = CApp::getInstance();
    retVal = oApp.getCmdLineArguments();
    if (Err::NoError == retVal)
    {
        retVal = oApp.init();
        if (Err::NoError == retVal)
        {
            retVal = oApp.run();
        }
    }
    oApp.handleErrorCode(retVal);

    logPrint(Info) << "Application end. Return code:" << retVal;
    CLogger::closeFile();
	return static_cast<int>(retVal);
}
