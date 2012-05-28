/*************************************************************************************************\
*                                                                                                 *
* "graphics.cpp" - Functions enabling use of Windows GDI without worrying about getting/releasing *
*                  device context and creating/destroying pens, brushes etc.                      *
*                                                                                                 *
*         Author - Tom McDonnell 2003                                                             *
*                                                                                                 *
\*************************************************************************************************/

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////

#include "graphics.h"
#include "lib_tom_cpp/geometry.h"

#include <assert.h>
#include   <math.h>

// GLOBAL VARIABLES ///////////////////////////////////////////////////////////////////////////////

extern HWND main_window_handle;   // defined in "winmain.c"

// STATIC GLOBAL VARIABLES ////////////////////////////////////////////////////////////////////////

static int maxx, maxy;

static HPEN old_pen, white_pen, gray_pen, red_pen, black_pen;
static HBRUSH old_brush, white_brush, gray_brush, red_brush, black_brush;
static HDC hdc;

// FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////

/*
 *
 */
void initGraphics(int width, int height)
{
   maxx = width;
   maxy = height;
}

/*
 *
 */
COLORREF getColor(int i)
{
   COLORREF c;

   switch (i)
   {
     case 0: c = RED;    break;
     case 1: c = GREEN;  break;
     case 2: c = BLUE;   break;
     case 3: c = YELLOW; break;
     case 4: c = PURPLE; break;
   }

   return c;
}

/*
 *
 */
void drawPixel(double x, double y, COLORREF c)
{
   assert(-1.0 <= x && x <= 1.0);
   assert(-1.0 <= y && y <= 1.0);

   // scaling
   x = ( x + 1.0) * (maxx / 2.0);
   y = (-y + 1.0) * (maxy / 2.0);

   hdc = GetDC(main_window_handle);
   SetPixel(hdc, (int)(x + 0.5), (int)(y + 0.5), c);
   ReleaseDC(main_window_handle, hdc);
}

void drawPixel(rec2vector p, COLORREF c)
{
   drawPixel(p.x, p.y, c);
}

/*
 *
 */
void drawLine(double x1, double y1, double x2, double y2, COLORREF c)
{
   assert(-1.0 <= x1 && x1 <= 1.0);
   assert(-1.0 <= y1 && y1 <= 1.0);
   assert(-1.0 <= x2 && x2 <= 1.0);
   assert(-1.0 <= y2 && y2 <= 1.0);

   // scaling
   x1 = ( x1 + 1.0) * (maxx / 2.0);
   y1 = (-y1 + 1.0) * (maxy / 2.0);
   x2 = ( x2 + 1.0) * (maxx / 2.0);
   y2 = (-y2 + 1.0) * (maxy / 2.0);

   HPEN tempPen = CreatePen(PS_SOLID, 0, c);

   hdc = GetDC(main_window_handle);

   old_pen = (HPEN)SelectObject(hdc, tempPen);

   MoveToEx(hdc, (int)(x1 + 0.5), (int)(y1 + 0.5), NULL);
   LineTo  (hdc, (int)(x2 + 0.5), (int)(y2 + 0.5));

   SelectObject(hdc, old_pen);

   ReleaseDC(main_window_handle, hdc);

   DeleteObject(tempPen);
}

void drawLine(rec2vector p1, rec2vector p2, COLORREF c)
{
   drawLine(p1.x, p1.y, p2.x, p2.y, c);
}

/*
 *
 */
void drawArrow(double x, double y, double angle, double length, double hLength, COLORREF c)
{
   assert(-1.0 <= x && x <= 1.0);
   assert(-1.0 <= y && y <= 1.0);
   assert( 0.0 <=  length &&  length <= 2.0);
   assert( 0.0 <= hLength && hLength <= 2.0);

   // scaling coordinates
   x = ( x + 1.0) * (maxx / 2.0);
   y = (-y + 1.0) * (maxy / 2.0);

   // scaling angle
   angle *= -1;

   // scaling lengths
   if (maxx == maxy)
   {
       length *= (maxx / 2.0);
      hLength *= (maxx / 2.0);
   }
   else
   {
      // TO DO: write scaling code
   }
   
   HPEN tempPen = CreatePen(PS_SOLID, 0, c);

   hdc = GetDC(main_window_handle);

   old_pen = (HPEN)SelectObject(hdc, tempPen);

   double x2 = x + length * cos(angle);
   double y2 = y + length * sin(angle);

   // draw arrow tail
   MoveToEx(hdc, (int)(x + 0.5), (int)(y + 0.5), NULL);
   LineTo(hdc, (int)(x2 + 0.5), (int)(y2 + 0.5));

   // draw arrow head
   if (hLength > 0.0)
   {
      LineTo(hdc, (int)(x2 + hLength * cos(angle - 0.75 * pi) + 0.5),
                  (int)(y2 + hLength * sin(angle - 0.75 * pi) + 0.5));
      MoveToEx(hdc, (int)(x2 + 0.5), (int)(y2 + 0.5), NULL);
      LineTo(hdc, (int)(x2 + hLength * cos(angle + 0.75 * pi) + 0.5),
                  (int)(y2 + hLength * sin(angle + 0.75 * pi) + 0.5));
   }

   SelectObject(hdc, old_pen);

   ReleaseDC(main_window_handle, hdc);

   DeleteObject(tempPen);
}

void drawArrow(rec2vector p, double angle, double length, double hLength, COLORREF c)
{
   drawArrow(p.x, p.y, angle, length, hLength, c);
}

void drawArrow(rec2vector p, pol2vector v, double hLength, COLORREF c)
{
   drawArrow(p.x, p.y, v.angle, v.r, hLength, c);
}

/*
 *
 */
void drawRect(double x1, double y1, double x2, double y2, COLORREF c)
{
   assert(-1.0 <= x1 && x1 <= 1.0);
   assert(-1.0 <= y1 && y1 <= 1.0);
   assert(-1.0 <= x2 && x2 <= 1.0);
   assert(-1.0 <= y2 && y2 <= 1.0);

   // scaling
   x1 = ( x1 + 1.0) * (maxx / 2.0);
   y1 = (-y1 + 1.0) * (maxy / 2.0);
   x2 = ( x2 + 1.0) * (maxx / 2.0);
   y2 = (-y2 + 1.0) * (maxy / 2.0);

   HPEN   tempPen   = CreatePen(PS_SOLID, 0, c);
   HBRUSH tempBrush = CreateSolidBrush(c);

   hdc = GetDC(main_window_handle);

   old_pen   =   (HPEN)SelectObject(hdc, tempPen);
   old_brush = (HBRUSH)SelectObject(hdc, tempBrush);

   Rectangle(hdc, (int)(x1 + 0.5), (int)(y1 + 0.5), (int)(x2 + 0.5), (int)(y2 + 0.5));

   SelectObject(hdc, old_pen);
   SelectObject(hdc, old_brush);

   ReleaseDC(main_window_handle, hdc);
}

void drawRect(rec2vector p1, rec2vector p2, COLORREF c)
{
   drawRect(p1.x, p1.y, p2.x, p2.y, c);
}

/*
 *
 */
void drawCircle(double x, double y, double r, COLORREF c)
{
   assert(-1.0 <= x && x <= 1.0);
   assert(-1.0 <= y && y <= 1.0);
   assert( 0.0 <= r && r <= 1.0);

   // scaling coordinates
   x = ( x + 1.0) * (maxx / 2.0);
   y = (-y + 1.0) * (maxy / 2.0);

   // scaling lengths
   if (maxx == maxy)
   {
      r *= (maxx / 2.0);
   }

   HPEN   tempPen   = CreatePen(PS_SOLID, 0, c);
   HBRUSH tempBrush = CreateSolidBrush(c);

   hdc = GetDC(main_window_handle);

   old_pen   = (HPEN)SelectObject(hdc, tempPen);
   old_brush = (HBRUSH)SelectObject(hdc, tempBrush);

   Ellipse(hdc, (int)(x - r + 0.5), (int)(y - r + 0.5), (int)(x + r + 0.5), (int)(y + r + 0.5));

   SelectObject(hdc, old_pen);
   SelectObject(hdc, old_brush);

   ReleaseDC(main_window_handle, hdc);
}

void drawCircle(rec2vector p, int r, COLORREF c)
{
   drawCircle(p.x, p.y, r, c);
}

/*
 *
 */
void drawText(double x, double y, char *messagePtr, int messageLength)
{
   // scaling
   x = ( x + 1.0) * (maxx / 2.0);
   y = (-y + 1.0) * (maxy / 2.0);

   GetDC(main_window_handle);
   SetBkMode(hdc, OPAQUE);
   SetBkColor(hdc, RGB(0, 0, 0));
   SetTextColor(hdc, RGB(255, 255, 255));
   TextOut(hdc, (int)(x + 0.5), (int)(y + 0.5), messagePtr, messageLength);
   ReleaseDC(main_window_handle, hdc);
}

void drawText(rec2vector p, char *messagePtr, int messageLength)
{
   drawText(p.x, p.y, messagePtr, messageLength);
}

/*****************************************END*OF*FILE*********************************************/
