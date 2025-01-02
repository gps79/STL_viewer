/**
 * @file CRenderer.cpp
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#include "CRenderer.h"
#include <GL/freeglut.h>
#include "CLogger.h"
#include "CTextOutput.h"
#include <iomanip>

using namespace std::literals::string_literals;

Err CRenderer::init(WNDPROC pMsgHandler)
{
    Err retVal{Err::NoError};

    retVal = createWindow(pMsgHandler);
    if (Err::NoError == retVal)
    {
        retVal = createRenderContext();
        if (Err::NoError == retVal)
        {
            int argc{0};
            char dummyArg[]{""};
            char *argv[]{dummyArg};
            glutInit(&argc, argv);
            GLenum glErr = glGetError();
            if (GL_NO_ERROR != glErr)
            {
                logPrint(Error) << "OpenGL error:" << glErr;
                retVal = Err::GlInitError;
            }
            else
            {
                logPrint(Info) << "OpenGL version: " << glGetString(GL_VERSION);
            }
        }
    }
    return retVal;
}

void CRenderer::destroy()
{
    logPrint(Trace) << "CRenderer destroy";
    if (m_hRenderContext)
    {
        wglDeleteContext(m_hRenderContext);
    }
}

Err CRenderer::createWindow(WNDPROC pMsgHandler)
{
    Err retVal{Err::NoError};

	constexpr int iInitialWindowWidth{800};
	constexpr int iInitialWindowHeight{600};
	constexpr const CHAR *szClassName{"GP"};
	constexpr const CHAR *szTitle{"STL viewer (c) Grzegorz Pietrusiak"};
	constexpr DWORD dwWindowStyle{WS_VISIBLE|WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS};

    logPrint(Trace) << "createWindow";
    HINSTANCE hInstance = GetModuleHandle(nullptr); // currently running EXE instance
	if (hInstance)
	{
        WNDCLASS  wndClass{}; // init the structure with zeros first
        wndClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wndClass.lpfnWndProc   = (WNDPROC)pMsgHandler;
        wndClass.hInstance     = hInstance;
        wndClass.lpszClassName = szClassName;
        wndClass.hCursor       = LoadCursor (nullptr, IDC_ARROW);
        if (RegisterClass(&wndClass))
        {
            const int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
            const int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
            int iInitialWindowPosX = (iScreenWidth-iInitialWindowWidth)/2;
            int iInitialWindowPosY = (iScreenHeight-iInitialWindowHeight)/2;
            m_hWindowHandle = CreateWindow(szClassName, szTitle, dwWindowStyle, iInitialWindowPosX, iInitialWindowPosY, iInitialWindowWidth, iInitialWindowHeight, nullptr, nullptr, hInstance, nullptr);
            if (!m_hWindowHandle)
            {
                logPrint(Error) << "Couldn't create the window";
                retVal = Err::CantCreateWindow;
            }
        }
        else
        {
            logPrint(Error) << "Couldn't register the window class";
            retVal = Err::CantRefgisterWindowClass;
        }
	}
	else
    {
        logPrint(Error) << "Couldn't create the window";
        retVal = Err::CantGetAppInstance;
    }

    return retVal;
}

Err CRenderer::createRenderContext()
{
    Err retVal{Err::NoError};

    logPrint(Trace) << "createRenderContext";
	// Get the window's device context
	m_hDeviceContext = GetDC(m_hWindowHandle);
	if (m_hDeviceContext)
    {
        // Choose a pixel format
        PIXELFORMATDESCRIPTOR pfd{}; // init the structure with zeros first
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;
        pfd.cDepthBits = 24;

        int iPixelFormat = ChoosePixelFormat(m_hDeviceContext, &pfd);
        if (iPixelFormat)
        {
            if (SetPixelFormat(m_hDeviceContext, iPixelFormat, &pfd))
            {
                m_hRenderContext = wglCreateContext(m_hDeviceContext);
                if (m_hRenderContext)
                {
                    if (!wglMakeCurrent(m_hDeviceContext, m_hRenderContext))
                    {
                        logPrint(Error) << "Couldn't bind the render context";
                        retVal = Err::CantBindRenderContext;
                    }
                }
                else
                {
                    logPrint(Error) << "Couldn't create a render context";
                    retVal = Err::CantCreateRenderContext;
                }
            }
            else
            {
                logPrint(Error) << "Couldn't set the pixel format";
                retVal = Err::CantSetPixelFormat;
            }
        }
        else
        {
            logPrint(Error) << "Couldn't find a pixel format";
            retVal = Err::CantFindPixelFormat;
        }
    }
    else
    {
        logPrint(Error) << "The window doesn't have a device context";
        retVal = Err::DeviceContext;
    }

    return retVal;
}

void CRenderer::setView(int iWidth, int iHeight)
{
    logPrint(Debug) << "setView:" << iWidth << "," << iHeight;
    m_iWidth = iWidth;
    m_iHeight = iHeight;
	glViewport(0, 0, iWidth, iHeight);
}

void CRenderer::clearScreen() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Err CRenderer::redrawWindow(const CModel &oModel)
{
    Err retVal{Err::NoError};

    if ((m_iWidth > 0) && (m_iHeight > 0))
    {
        // draw 3D object
        constexpr double dCameraViewAngle = 40.0;
        const double dAspectRatio = static_cast<double>(m_iWidth) / static_cast<double>(m_iHeight);
        constexpr float dNearZSceneClipping = 0.1;
        constexpr float dFarZSceneClipping = 400.0;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(dCameraViewAngle, dAspectRatio, dNearZSceneClipping, dFarZSceneClipping);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        clearScreen();
        glEnable(GL_DEPTH_TEST);
        glPolygonOffset(1.0f, 2); // used for wireframes; see http://www.cs.rit.edu/~ncs/Courses/570/UserGuide/OpenGLonWin-14.html
        drawObject(oModel);

        // draw 2D part of the screen
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, m_iWidth, m_iHeight, 0, 0, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        drawFlatElements(oModel);

        // let the FPS counter know that we finished rendering a frame
        m_oFpsCounter.nextFrame();

        SwapBuffers(m_hDeviceContext);
    }
#ifdef DEBUG
    // (Only in DEBUG build) once a frame check for OpenGL errors. Application is stopped if any OpenGL error occurs.
    GLenum glErr = glGetError();
    if (GL_NO_ERROR != glErr)
    {
        logPrint(Error) << "OpenGL error:" << glErr;
        retVal = Err::GlError;
    }
#endif

    return retVal;
}

void CRenderer::drawObject(const CModel &oModel)
{
    switch (m_drawMode)
    {
        case DrawMode::wireframe: // wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;

        case DrawMode::filledWires: // filled wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_POLYGON_OFFSET_FILL);
            break;

        case DrawMode::shading:
        default: // SHADING: fill mode with shading by default
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_LIGHTING); //Enable lighting
            // 1. ambient light
            GLfloat afAmbientColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; // ambient color RGBA
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, afAmbientColor);

            // 2. Add positioned light
            glEnable(GL_LIGHT0); //Enable light #0
            glEnable(GL_NORMALIZE); //Automatically normalize normals
            // material properties
            GLfloat afAmbient[] = {0.25f, 0.148f, 0.06475f, 1.0f};
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, afAmbient);
            GLfloat afDiffuse[] = {0.4f, 0.2368f, 0.1036f, 1.0f};
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, afDiffuse);
            GLfloat afSpecular[] = {0.774597f, 0.458561f, 0.200621f, 1.0f};
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, afSpecular);
            GLfloat afShininess = 10.8f;
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, afShininess);
            glEnable(GL_COLOR_MATERIAL); // enable material properties
            glColorMaterial(GL_FRONT, GL_AMBIENT); // material properties are defined by vertices colors
            break;
    }

    constexpr float fPositionScale = 0.01f;
    glTranslatef(fPositionScale*static_cast<float>(m_iViewPosX), fPositionScale*static_cast<float>(m_iViewPosY), m_fZoom);
    glMultMatrixf(m_oModelViewOrientation.toMatrix().data());
    glRotatef(static_cast<float>(m_iFrame), 0.0f, 1.0f, 0.0f ); // 3D model animation around Y-axis
    glScalef(4.0f, 4.0f, 4.0f); // scale whole object to fill in the view

    if (m_bAnime)
        ++m_iFrame;

    switch (m_drawMode)
    {
        case DrawMode::wireframe:
            glColor3f(0.9f, 0.9f, 0.5f);
            break;

        case DrawMode::filledWires:
            break;

        default: // SHADING
            glColor3f(0.81f, 0.71f, 0.23f); // old gold color
            break;
    }

    int iFacetNum{0};
    for (const auto &facet : oModel.getFacets())
    {
        if (m_u16SkipTriangles)
        {
            ++iFacetNum;
            if (iFacetNum % m_u16SkipTriangles) continue;
        }
        const CVector3d &normal = facet.normal;
        const CVector3d &p1 = facet.p1;
        const CVector3d &p2 = facet.p2;
        const CVector3d &p3 = facet.p3;
        glBegin(GL_TRIANGLES);
        glNormal3f(normal.m_fX, normal.m_fY, normal.m_fZ);
        glVertex3f(p1.m_fX, p1.m_fY, p1.m_fZ);
        glVertex3f(p2.m_fX, p2.m_fY, p2.m_fZ);
        glVertex3f(p3.m_fX, p3.m_fY, p3.m_fZ);
        glEnd();
        if (DrawMode::filledWires == m_drawMode)
        {
            glColor3f(0.9f, 0.9f, 0.5f); // pale yellow
            glBegin(GL_LINE_LOOP);
            glVertex3f(p1.m_fX, p1.m_fY, p1.m_fZ);
            glVertex3f(p2.m_fX, p2.m_fY, p2.m_fZ);
            glVertex3f(p3.m_fX, p3.m_fY, p3.m_fZ);
            glEnd();
            glColor3f(0.3f, 0.3f, 0.3f); // dark gray
        }
    }

    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
}

void CRenderer::drawFlatElements(const CModel &oModel)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glColor4f(0.0f, 0.5f, 0.5f, 0.3f);
    glVertex2d(0, 0);
    glVertex2d(200, 0);
    glVertex2d(200, 220);
    glVertex2d(0, 220);
    glEnd();
    glDisable(GL_BLEND);
    glColor3f(0.0f, 0.5f, 0.5f);
    CTextOutput to(m_iWidth, m_iHeight);
    to.setFont(5);
    to.setCursorPos(0, 0);
    to.setSpacing(0);

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << m_oFpsCounter.getFps() << " FPS";
    to.printLn(stream.str());
    to.printLn("Name:"s + oModel.getModelName());
    to.printLn(std::to_string(oModel.getFacets().size()) + " polygons");
    stream.str(std::string());
    stream << "Display mode: "s << m_drawMode;
    to.printLn(stream.str());
    to.printLn("");
    to.printLn("Menu:");
    to.printLn("Esc - Exit");
    to.printLn("LMB - rotate in XY axes");
    to.printLn("RMB - drag object");
    to.printLn("MMB - rotate in Z axis");
    to.printLn("Mouse scroll - zoom");
    to.printLn("SPACE - pause animation");
    to.printLn("TAB - change display mode");
    to.printLn("r - reset view");
    to.printLn("s - skip displaying some polygons");
    to.printLn("     to navigate faster");
    to.printLn("x,y,z - rotate model");
}

void CRenderer::resetViewState()
{
    logPrint(Debug) << "resetViewState";
    m_fZoom = -8.0f;
    m_iViewPosX = 0;
    m_iViewPosY = 0;
    m_iFrame = 0;
    m_u16SkipTriangles = 0;
    m_oModelViewOrientation.reset();
}

void CRenderer::zoom(float fZoomRatio)
{
    logPrint(Debug) << "zoom:" << fZoomRatio;
    constexpr float fScale{0.5f};
    m_fZoom += fScale * fZoomRatio;
}

void CRenderer::setNextDrawMode()
{
    switch (m_drawMode)
    {
        case DrawMode::shading:
            m_drawMode = DrawMode::wireframe;
            break;

        case DrawMode::wireframe:
            m_drawMode = DrawMode::filledWires;
            break;

        case DrawMode::filledWires:
            m_drawMode = DrawMode::shading;
            break;

        default: // from unknown state go to shading
            m_drawMode = DrawMode::shading;
            break;
    }
    logPrint(Debug) << "setNextDrawMode:" << m_drawMode;
}

void CRenderer::setNextSkipTrianglesMode()
{
    switch (m_u16SkipTriangles)
    {
        case 0:
            m_u16SkipTriangles = 2;
            break;

        case 2:
            m_u16SkipTriangles = 8;
            break;

        case 8:
            m_u16SkipTriangles = 32;
            break;

        case 32:
            m_u16SkipTriangles = 128;
            break;

        case 128:
        default:
            m_u16SkipTriangles = 0;
            break;
    }
    logPrint(Debug) << "setNextSkipTrianglesMode:" << m_u16SkipTriangles;
}

void CRenderer::rotateX(float fAngle)
{
    logPrint(Debug) << "rotateX:" << fAngle;
    m_oModelViewOrientation.multiple(std::sin(fAngle), 0, 0, std::cos(fAngle));
}

void CRenderer::rotateY(float fAngle)
{
    logPrint(Debug) << "rotateY:" << fAngle;
    m_oModelViewOrientation.multiple(0, std::sin(fAngle), 0, std::cos(fAngle));
}

void CRenderer::rotateZ(float fAngle)
{
    logPrint(Debug) << "rotateZ:" << fAngle;
    m_oModelViewOrientation.multiple(0, 0, std::sin(fAngle), std::cos(fAngle));
}

std::ostream& operator<<(std::ostream& stream, const CRenderer::DrawMode& o)
{
    std::string sStr;

    switch (o)
    {
        case CRenderer::DrawMode::shading:
            sStr = "shaded object";
            break;

        case CRenderer::DrawMode::wireframe:
            sStr = "wireframe";
            break;

        case CRenderer::DrawMode::filledWires:
            sStr = "outline";
            break;

        default:
            sStr = static_cast<int>(o);
            break;
    }

	stream << sStr;
	return stream;
}
