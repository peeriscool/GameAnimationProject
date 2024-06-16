#pragma warning(disable:28251)
#pragma warning(disable:28159)

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include "glad.h"
#undef APIENTRY
#include <windows.h>

#include <iostream>
#include "Application.h"
//#include "Chapter06Sample01.h"
#include "MyDemoApp.h"
//#include "Chapter09Sample02.h"
#include "globals.h"
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif
bool keys[256] = { false };
MyApp Myapp;
int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int);
LRESULT CALLBACK Wndproc(HWND, UINT, WPARAM, LPARAM);
//using pragma statement to link subsystems console and windows
#if _DEBUG
	#pragma comment (linker, "/subsystem:console")
	int main(int argc, const char** argv)
	{
		return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWDEFAULT);
	}
#else
#pragma comment (linker,"/subsystem:windows")
#endif
#pragma comment(lib,"opengl32.lib") //openGl 
#pragma comment(lib,"user32.lib") 
#pragma comment(lib,"gdi32.lib")
//WGL/opengl context
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091 
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092 
#define WGL_CONTEXT_FLAGS_ARB 0x2094 
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001 
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126 
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC, HGLRC, const int*);

typedef const char* (WINAPI* PFNWGLGETEXTENSIONSSTRINGEXTPROC) (void); 
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC) (int); 
typedef int (WINAPI* PFNWGLGETSWAPINTERVALEXTPROC) (void);


Application* gApplication =0 ;
GLuint gVertexArrayObject = 0;

//define WinMain
int WINAPI WinMain(HINSTANCE HInstance, HINSTANCE hPrevInstance, PSTR szCMDLine, int iCmdShow)
{
	gApplication = new MyApp(); // //new Chapter09Sample02(); //new Chapter09Sample01();// new Chapter06Sample01;//new Application();

	//setting window properties
	WNDCLASSEX wndclass; //https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = Wndproc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = HInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = 0;
	wndclass.lpszClassName = L"Win32 Game Window"; //https://stackoverflow.com/questions/21864210/win32-application-programming-c-error-with-parameter-type-lpcwstr
	RegisterClassEx(&wndclass);


	//creating rectangle viewport
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeigth = GetSystemMetrics(SM_CYSCREEN);
	int clientWidth = 1280;
	int clientHeight = 720;
	RECT windowRect;
	SetRect(&windowRect,
	(screenWidth/2) - (clientWidth / 2) ,
	(screenHeigth / 2) - (clientHeight / 2) ,
	(screenWidth / 2) + (clientWidth / 2) ,
	(screenHeigth / 2) + (clientHeight / 2) );
	
	//minimized / maximized
	DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX); //WS_THICKFRAMe To reisze
	AdjustWindowRectEx(&windowRect, style, FALSE, 0);
	HWND hwnd = CreateWindowEx(0, wndclass.lpszClassName,L"GameWindow", style, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, HInstance, szCMDLine);
	HDC hdc = GetDC(hwnd);

	//PixelFormatDescriptor
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); 
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);
	
	//temp opengl context
	HGLRC tempRC = wglCreateContext(hdc);
	wglMakeCurrent(hdc, tempRC);
	PFNWGLCREATECONTEXTATTRIBSARBPROC
		wglCreateContextAttribsARB = NULL;
	wglCreateContextAttribsARB =
		(PFNWGLCREATECONTEXTATTRIBSARBPROC)
		wglGetProcAddress("wglCreateContextAttribsARB");
	
	const int attribList[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3, 
		WGL_CONTEXT_MINOR_VERSION_ARB, 3, 
		WGL_CONTEXT_FLAGS_ARB, 0, 
		WGL_CONTEXT_PROFILE_MASK_ARB, 
		WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0, }; 

	//set OpenGl context
	HGLRC hglrc = wglCreateContextAttribsARB(hdc, 0, attribList);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempRC);
	wglMakeCurrent(hdc, hglrc);

	if (!gladLoadGL())
	{
		std::cout << "Could not init Glad\n";
	}
	else
	{
		std::cout << "OpenGL Version" << GLVersion.major << "." << GLVersion.minor << "\n";
	}
	PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
	bool swapControlSupported = strstr(_wglGetExtensionsStringEXT(), "WGL_EXT_swap_control") != 0;
	int vsynch = 0;
	if (swapControlSupported)
	{
			PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
			PFNWGLSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
			if (wglSwapIntervalEXT(1))
			{
				std::cout << "Enabled Vsynch! \n";
				vsynch = wglGetSwapIntervalEXT(1);
			}
			else
			{
				std::cout << "No Vsync available\n";
			}
	}
	else
	{
		//!swapControllSupported
		std::cout << "WGL_EXT_swap_control not supported\n";
	}

	glGenVertexArrays(1, &gVertexArrayObject);
	glBindVertexArray(gVertexArrayObject);
	
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	gApplication->Initialize(); //https://www.geeksforgeeks.org/arrow-operator-in-c-c-with-examples/

	DWORD LastTick = GetTickCount64();
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	
		DWORD ThisTick = GetTickCount64();
		float dt = float(ThisTick - LastTick) * 0.001f;
		LastTick = ThisTick;
		if (gApplication != 0) //"Game" loop
		{
			gApplication->Update(dt);
		}

		if (gApplication != 0) 
		{
			RECT clientRect; 
			GetClientRect(hwnd, &clientRect); 
			clientWidth = clientRect.right - clientRect.left; 
			clientHeight = clientRect.bottom - clientRect.top; 
			glViewport(0, 0, clientWidth, clientHeight); 
			glEnable(GL_DEPTH_TEST); glEnable(GL_CULL_FACE);
			glPointSize(5.0f); glBindVertexArray(gVertexArrayObject); 
			glClearColor(0.5f, 0.6f, 0.7f, 1.0f); 
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
			float aspect = (float)clientWidth / (float)clientHeight; gApplication->Render(aspect); 
		}
		if (gApplication != 0)
		{
			SwapBuffers(hdc);
			if (vsynch != 0)
			{
				glFinish();
			}
		}
	} //end of "game" loop

	if (gApplication != 0)
	{
		std::cout << "Expected application to be null on exit\n";
			delete gApplication;
	}
	return (int)msg.wParam;
}
LRESULT CALLBACK Wndproc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lparam)
{
	switch (iMsg)
	{	
	case WM_CLOSE:
		if (gApplication != 0)
		{
			gApplication->Shutdown();
			delete gApplication;
			gApplication = 0;
			DestroyWindow(hwnd);
		}
		else 
		{
			std::cout << "already shut down!\n";
		}
		break;
		case WM_DESTROY:
		if (gVertexArrayObject != 0)
		{
			HDC hdc = GetDC(hwnd);
			HGLRC hglrc = wglGetCurrentContext();

			glBindVertexArray(0);
			glDeleteVertexArrays(1, &gVertexArrayObject);
			gVertexArrayObject = 0;

			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hglrc);
			ReleaseDC(hwnd, hdc);
			PostQuitMessage(0);
		}
		else
		{
			std::cout << "Multiple destroy messages\n";
		}
		break;
		//case WM_PAINT:
		//case WM_ERASEBKGND:
		case WM_MOUSEMOVE: 
		{
			int xPos = GET_X_LPARAM(lparam);//GET_X_LPARAM(lparam);
			int yPos = GET_Y_LPARAM(lparam);
			gApplication->UpdateMousePosition(xPos, yPos);
			//std::cout << "Mouse position: (" << xPos << ", " << yPos << ")\n";
			// Handle mouse move (e.g., update cursor position)
			// Example: UpdateCursorPos(xPos, yPos);
			break;
		}
		case WM_LBUTTONDOWN: {
			// Handle left mouse button down
			// Example: ProcessLeftButtonDown();
			break;
		}
		case WM_LBUTTONUP: {
			// Handle left mouse button up
			// Example: ProcessLeftButtonUp();
			break;
		}
		case WM_KEYDOWN:
			keys[wParam] = true; // Mark the key as pressed
			break;

		case WM_KEYUP:
			keys[wParam] = false; // Mark the key as released
			break;				 
			return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lparam);
}
