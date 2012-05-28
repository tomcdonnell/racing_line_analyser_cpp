/*************************************************************************************************\
*                                                                                                 *
* "graphics.h" - Header file for "graphics.cpp"                                                   *
*                                                                                                 *
*       Author - Tom McDonnell 2003                                                               *
*                                                                                                 *
*         Info : Coordinates are scaled so that top left of window is (x = -1, y = 1),            *
*                and bottom right is (x = 1, y = -1).  Centre is thus (x - 0, y = 0).             *
*                All lengths are similarly scaled.                                                *
*                                                                                                 *
\*************************************************************************************************/

#ifndef GRAPHICS_H
#define GRAPHICS_H

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////

#include "lib_tom_cpp/vector.h"

#include <windows.h>

// DEFINES ////////////////////////////////////////////////////////////////////////////////////////

#define WHITE  RGB(255, 255, 255)
#define GRAY   RGB(100, 100, 100)
#define BLACK  RGB(  0,   0,   0)
#define RED    RGB(255,   0,   0)
#define GREEN  RGB(  0, 255,   0)
#define BLUE   RGB(  0,   0, 255)
#define YELLOW RGB(255, 255,   0)
#define PURPLE RGB(255,   0, 255)

// FUNCTION DECLARATIONS //////////////////////////////////////////////////////////////////////////

void initGraphics(int maxx, int maxy);

COLORREF getColor(int);

void drawPixel(double x, double y, COLORREF);
void drawPixel(rec2vector,         COLORREF);

void drawLine(double x1, double y1, double x2, double y2, COLORREF);
void drawLine(rec2vector p1,        rec2vector p2,        COLORREF);

void drawArrow(double x, double y, double angle, double length, double hLength, COLORREF);
void drawArrow(rec2vector,         double angle, double length, double hLength, COLORREF);
void drawArrow(rec2vector,         pol2vector,                  double hLength, COLORREF);

void drawRect(double x1, double y1, double x2, double y2, COLORREF);
void drawRect(rec2vector p1,        rec2vector p2,        COLORREF);

void drawCircle(double x, double y, double r, COLORREF);
void drawCircle(rec2vector,         double r, COLORREF);

void drawText(double x, double y, char *, int messageLength);
void drawText(rec2vector,         char *, int messageLength);

#endif

/*****************************************END*OF*FILE*********************************************/
