/*************************************************************************************************\
*                                                                                                 *
* "main.cpp" - Entry point and main functions for "Racing Line Analyser" program.                 *
*                                                                                                 *
*     Author - Tom McDonnell 2003                                                                 *
*                                                                                                 *
\*************************************************************************************************/

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include  <assert.h>

#include        "gui.h"
#include   "graphics.h"
#include "racingline.h"

// DEFINES ////////////////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

// GLOBAL VARIABLES ///////////////////////////////////////////////////////////////////////////////

HWND main_window_handle = NULL;

// STATIC GLOBAL VARIABLES ////////////////////////////////////////////////////////////////////////

static const int width = 500, height = 500; // dimensions of window to be created

// FUNCTION DECLARATIONS //////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // event handler for main window

// FUNCTION DEFINITIONS ///////////////////////////////////////////////////////////////////////////

/*
 * Main function.  Creates window and contains main event loop.
 */
int WINAPI WinMain(HINSTANCE hinstance,
                   HINSTANCE hprevinstance,
                   LPSTR     lpcmdline,
                   int       ncmdshow)
{
   WNDCLASS main;
   MSG msg;       // generic message used in main event loop

   // fill in main window class stucture
   main.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
   main.lpfnWndProc   = WindowProc;
   main.cbClsExtra    = 0;
   main.cbWndExtra    = 0;
   main.hInstance     = hinstance;
   main.hIcon         = LoadIcon(NULL, IDI_APPLICATION); // should have own icon
   main.hCursor       = LoadCursor(NULL, IDC_ARROW);
   main.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
   main.lpszMenuName  = NULL;
   main.lpszClassName = "mainWindowClass";

   // register the window class
   if (!RegisterClass(&main))
     return(0);

   // create the window
   main_window_handle 
     = CreateWindow("mainWindowClass",      // class
                    "Racing Line Analyser", // title
                    WS_SYSMENU | WS_CAPTION | WS_VISIBLE, // style
                    CW_USEDEFAULT,          // x pos
                    CW_USEDEFAULT,          // y pos
                    width  + 6,             // width (+6 allows for border)
                    height + 6 + 26,        // height (+26 allows for title)
                    NULL,                   // handle to parent 
                    NULL,                   // handle to menu
                    hinstance,              // instance
                    NULL);                  // creation parms

   if (main_window_handle == NULL)
     return(0);

   initGraphics(width, height);
   initGUI();
   drawWindow();

   // enter main event loop
   while (true)
   {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
         if (msg.message == WM_QUIT)
           break;
         TranslateMessage(&msg); // translate any accelerator keys
         DispatchMessage(&msg);  // send message to window proc
      }
   }
   return(msg.wParam); // return to Windows
}

/*
 *
 */
void wait(int tmsec)
{
   assert(tmsec > 0);

   unsigned int waitEndTime = GetTickCount() + tmsec;

   MSG msg;

   // duplicated main event loop
   while (true)
   {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
         if (msg.message == WM_QUIT)
           break;
         TranslateMessage(&msg); // translate any accelerator keys
         DispatchMessage(&msg);  // send message to window proc
      }
      // main processing not dependant on external events goes here
      if (GetTickCount() > waitEndTime)
        break;
   }
}

/*
 * Message handler for main window.
 */
LRESULT CALLBACK WindowProc(HWND   hwnd, 
						          UINT   msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
   HDC          hdc;
   PAINTSTRUCT  ps;       // used in WM_PAINT
   rec2vector   mousePos; // used in WM_LBUTTONDOWN

   // what is the message
   switch (msg)
   {
    case WM_CREATE:
		// do initialization stuff here
		return(0);
      break;

    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      // repaint window PROBLEM: not all of window needs to be repainted
      // must use 'ps.rcPaint' rectangle structure
      drawWindow();
		return(0);
      break;

    case WM_LBUTTONDOWN:
      mousePos.x = LOWORD(lparam);
      mousePos.y = HIWORD(lparam);

      // scale to between -1 and 1 for compatability with gui code
      mousePos.x =   mousePos.x * (2.0 / width ) - 1;
      mousePos.y = -(mousePos.y * (2.0 / height) - 1);

      processLeftClick(mousePos);
      break;

    case WM_MOUSEMOVE:
      mousePos.x = LOWORD(lparam);
      mousePos.y = HIWORD(lparam);

      // scale to between -1 and 1 for compatability with gui code
      mousePos.x =   mousePos.x * (2.0 / width ) - 1;
      mousePos.y = -(mousePos.y * (2.0 / height) - 1);

      processMouseMove(mousePos);
      break;

    case WM_DESTROY:
		// kill the application
	 	PostQuitMessage(0);
 		return(0);
      break;
   }

   // use default windowProc to process any messages not taken care of
   return (DefWindowProc(hwnd, msg, wparam, lparam));
}

/*****************************************END*OF*FILE*********************************************/
