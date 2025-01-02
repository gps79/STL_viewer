/**
 * @file CApp.cpp
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#include "common.h"
#include "CLogger.h"
#include "CApp.h"
#include "CStlLoader.h"

using namespace std::literals::string_literals;

Err CApp::getCmdLineArguments()
{
    Err retVal{Err::NoError};

    int iArgCount{0};
    auto szaArgList = CommandLineToArgvW(GetCommandLineW(), &iArgCount);
    if (nullptr != szaArgList)
    {
        if (2 == iArgCount)
        {
            logPrint(Trace) << "Reading command line arguments";
            int iArg1Len = lstrlenW(szaArgList[1]);
            int iStringLen = WideCharToMultiByte(CP_ACP, 0, szaArgList[1], iArg1Len, nullptr, 0, nullptr, nullptr);
            std::string sInputFileName;
            sInputFileName.resize(iStringLen);
            WideCharToMultiByte(CP_ACP, 0, szaArgList[1], iArg1Len, &sInputFileName[0], iStringLen, nullptr, nullptr);
            LocalFree(szaArgList);
            logPrint(Debug) << "Input file:\"" << sInputFileName << "\"";
            setFileName(sInputFileName);
        }
        else
        {
            logPrint(Error) << "One command line argument is expected";
            retVal = Err::MissingArg;
        }
    }
    else
    {
        logPrint(Error) << "Unable to parse command line";
        retVal = Err::CmdLineParse;
    }

    return retVal;
}

void CApp::handleErrorCode(Err errorCode) const
{
    std::string sMessage;
    /**
     * Special treatment of selected errors should be implemented here.
     * All errors which need user attention should raise MessageBox() with proper information.
     * All other errors are treated the same simple way, i.e. the user will see "Application error" with error code in the MessageBox.
     */
    switch (errorCode)
    {
        case Err::MissingArg:
            MessageBox(nullptr, "USAGE: stl_viewer.exe <file.stl>", "Error", MB_OK);
            break;

        case Err::InvalidStlFile:
            MessageBox(nullptr, "STL file is broken", "Error:", MB_OK|MB_ICONERROR);
            break;

        case Err::FileNotFound:
            sMessage = "File not found: "s + m_sInputFileName;
            MessageBox(nullptr, sMessage.c_str(), "Error:", MB_OK|MB_ICONERROR);
            break;

        case Err::MemAlloc:
            MessageBox(nullptr, "Not enough memory", "Error:", MB_OK|MB_ICONERROR);
            break;

        case Err::NoError:
            break;

        default:
            logPrint(Error) << "Application error: " << errorCode;
            sMessage = "Application error: "s + std::to_string(static_cast<int>(errorCode));
            MessageBox(nullptr, sMessage.c_str(), "Error", MB_OK);
            break;
    }
}

Err CApp::init()
{
    Err retVal{Err::NoError};

    retVal = loadFile();
    if (Err::NoError == retVal)
    {
        retVal = m_oRenderer.init(messageHandler);
        if (Err::NoError == retVal)
        {
            m_hWindowHandle = m_oRenderer.getWindowHandle();
        }
    }

    return retVal;
}

long int WINAPI CApp::messageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int iRetVal{1};
	CApp &oApp = CApp::getInstance();

	switch (uMsg)
	{
        case WM_PAINT:
            ValidateRect(hWnd, nullptr); // Remove the entire client area of the window from the update region
            break;

        case WM_SIZE:
            {
                int iWindowWidth = LOWORD(lParam);
                int iWindowHeight = HIWORD(lParam);
                oApp.m_oRenderer.setView(iWindowWidth, iWindowHeight);
            }
            break;

		case WM_CLOSE:
			PostQuitMessage(0);
			iRetVal = 0;
			break;

		case WM_KEYDOWN:
			oApp.handleKeyPressed(wParam);
			break;

		case WM_DESTROY:
		    wglMakeCurrent(nullptr, nullptr);
		    oApp.m_oRenderer.destroy();
            break;

		case WM_MOUSEWHEEL:
            {
                int16_t i16Delta = GET_WHEEL_DELTA_WPARAM(wParam); // read wheel distance traveled
                float fZoomRatio = i16Delta / WHEEL_DELTA; // to normalize the data we need to divide by the multiplier
                oApp.m_oRenderer.zoom(fZoomRatio);
            }
            break;

        case WM_SETFOCUS:
            oApp.setWindowFocus(true);
            break ;

        case WM_KILLFOCUS:
            oApp.setWindowFocus(false);
            break;

		default:
			iRetVal = DefWindowProc(hWnd, uMsg, wParam, lParam);
            break;
	}
	return iRetVal;
}

int CApp::updateMessageQueue()
{
    int iRetVal{1};
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE) == TRUE)
	{
		if (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);        // Will go to MessageHandler() above
		}
		else
		{
			DestroyWindow(m_hWindowHandle);  // Window was closed by the user
			iRetVal = 0;
			break;
		}
	}
	return iRetVal;
}


Err CApp::loadFile()
{
    Err retVal{Err::NoError};
    CStlLoader oStlLoader;

    retVal = oStlLoader.loadFile(m_sInputFileName, m_oModel);
    if (Err::NoError == retVal)
    {
        m_oModel.normalizeModel();
    }

    return retVal;
}

Err CApp::run()
{
    Err retVal{Err::NoError};

    while (updateMessageQueue())
    {
        POINT mousePosition{0, 0};
        GetCursorPos(&mousePosition);
        ScreenToClient(m_hWindowHandle, &mousePosition);
        if (getWindowFocus())
        {
            if (GetAsyncKeyState(VK_LBUTTON))
            {
                handleLMBPressed(mousePosition.x, mousePosition.y);
            }
            else
            {
                handleLMBReleased();
            }
            if (GetAsyncKeyState(VK_MBUTTON))
            {
                handleMMBPressed(mousePosition.x, mousePosition.y);
            }
            else
            {
                handleMMBReleased();
            }
            if (GetAsyncKeyState(VK_RBUTTON))
            {
                handleRMBPressed(mousePosition.x, mousePosition.y);
            }
            else
            {
                handleRMBReleased();
            }
        }
        retVal = m_oRenderer.redrawWindow(m_oModel);
        if (Err::NoError != retVal)
        {
            logPrint(Debug) << "Closing window due to an error";
            PostQuitMessage(0);
        }
    }

	return retVal;
}

void CApp::handleKeyPressed(int iKey)
{
    /* keyboard codes according to:
     * https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx
     * 0x30-0x39 : keys '0'-'9'
     * 0x41 A, 0x45 E, 0x49 I, 0x4E N, 0x53 S, 0x57 W, 0x5A Z
     * VK_F1-VK_F10 0x70-0x79 F1-F10 key, VK_F11 0x7A F11 key, VK_F12 0x7B F12 key
     */
	switch (iKey)
	{
		case VK_ESCAPE:
            logPrint(Debug) << "Esc key pressed";
			PostQuitMessage(0);
			break;

		case VK_SPACE: //' ':
			m_oRenderer.togglePlayAnimation();
            break;

		case 0x58: // 'x'
			m_oModel.rotateX();
            break;

		case 0x59: // 'y'
			m_oModel.rotateY();
            break;

		case 0x5A: // 'z'
			m_oModel.rotateZ();
            break;

		case VK_TAB: //TAB:
		    m_oRenderer.setNextDrawMode();
            break;

		case 0x52: //'r': reset view state
            m_bRmbDragging = false;
            m_bMmbDragging = false;
            m_bLmbDragging = false;
            m_iLmbDragMouseStartPosX = 0;
            m_iLmbDragMouseStartPosY = 0;
            m_oRenderer.resetViewState();
            break;

		case 0x53: //'s': skip displaying some triangles
            m_oRenderer.setNextSkipTrianglesMode();
            break;

		default: // no action for all other keys
            break;
	}
}

void CApp::handleLMBPressed(int iMouseX, int iMouseY)
{
    if (!m_bLmbDragging)
    {
        logPrint(Debug) << "LMB start: " << iMouseX << "," << iMouseY;
        m_bLmbDragging = true;
    }
    else
    {
        logPrint(Debug) << "LMB continue: " << iMouseX << "," << iMouseY;
        // left-right mouse movement rotates the object around the Y-axis
        // up-down mouse movement rotates the object around the X-axis
        constexpr float fRotAngleUnit = 0.3f*2.0f*M_PI/360.0f; // 0.3 degree per mouse position unit
        float fAngleY = -fRotAngleUnit*(iMouseX - m_iLmbDragMouseStartPosX); // positive mouse X change causes rotation by negative angle around Y axis
        float fAngleX = fRotAngleUnit*(iMouseY - m_iLmbDragMouseStartPosY) * -1.0f; // the value is negated because mouse Y-coordinate is flipped comparing to GL's Y-coordinate
        m_oRenderer.rotateX(fAngleX);
        m_oRenderer.rotateY(fAngleY);
    }

    m_iLmbDragMouseStartPosX = iMouseX;
    m_iLmbDragMouseStartPosY = iMouseY;
}

void CApp::handleLMBReleased()
{
    if (m_bLmbDragging)
    {
        logPrint(Debug) << "LMB stop";
        m_bLmbDragging = false;
    }
}

void CApp::handleMMBPressed(int iMouseX, int iMouseY)
{
    if (!m_bMmbDragging)
    {
        logPrint(Debug) << "MMB start: " << iMouseX << "," << iMouseY;
        m_bMmbDragging = true;
    }
    else
    {
        logPrint(Debug) << "MMB continue: " << iMouseX << "," << iMouseY;
        constexpr float fRotAngleUnit = 0.3f*2.0f*M_PI/360.0f; // 0.3 degree per mouse position unit
        float fAngleZ = fRotAngleUnit*(iMouseX - m_iMmbDragMouseStartPosX);
        m_oRenderer.rotateZ(fAngleZ);
    }

    m_iMmbDragMouseStartPosX = iMouseX;
    m_iMmbDragMouseStartPosY = iMouseY;
}

void CApp::handleMMBReleased()
{
    if (m_bMmbDragging)
    {
        logPrint(Debug) << "MMB stop";
        m_bMmbDragging = false;
    }
}

void CApp::handleRMBPressed(int iMouseX, int iMouseY)
{
    if (!m_bRmbDragging)
    {
        logPrint(Debug) << "RMB start: " << iMouseX << "," << iMouseY;
        m_bRmbDragging = true;
    }
    else
    {
        logPrint(Debug) << "RMB continue: " << iMouseX << "," << iMouseY;
        int iViewPosX = iMouseX - m_iRmbDragMouseStartPosX;
        int iViewPosY = (iMouseY - m_iRmbDragMouseStartPosY) * -1; // the value is negated because mouse Y-coordinate is flipped comparing to GL's Y-coordinate
        m_oRenderer.moveViewPos(iViewPosX, iViewPosY);
    }
    m_iRmbDragMouseStartPosX = iMouseX;
    m_iRmbDragMouseStartPosY = iMouseY;
}

void CApp::handleRMBReleased()
{
    if (m_bRmbDragging)
    {
        logPrint(Debug) << "MMB stop";
        m_bRmbDragging = false;
    }
}

