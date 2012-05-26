/*************************************************************************************************\
*                                                                                                 *
* "gui.cpp" -                                                                                     *
*                                                                                                 *
*    Author - Tom McDonnell 2003                                                                  *
*                                                                                                 *
\*************************************************************************************************/

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////

#include        "gui.h"
#include   "graphics.h"
#include "racingline.h"

#include   <math.h>
#include <assert.h>

// STATIC GLOBAL VARIABLES ////////////////////////////////////////////////////////////////////////

static enum {moveTrkE, moveTrkC, moveTrkX, normal} mouseMoveMode = normal;

// variables defining racetrack corner
static rec2vector trkE(-0.5, -1.0), // track screen entry  point (default)
                  trkC(-0.5,  0.5), // track corner centre point (default)
                  trkX( 1.0,  0.5), // track screen exit   point (default)
                  trkA;             // track apex

// variables defining racing line
static racingLine rL[5];
static       int   nRacingLines;
static const int maxRacingLines = 5;

static rect simBut,  clrBut,  accBut,  // simulate, clear & acceleration direction buttons
            trkButE, trkButC, trkButX; // track entry, centre, & exit buttons

// STATIC FUNCTION DECLARATIONS ///////////////////////////////////////////////////////////////////

static void drawRaceTrack(COLORREF);
static bool insideRect(rec2vector, rect);
static void surrPointWithRect(rec2vector, rect &, double);
static rec2vector lineIntersectScrBoundary(rec2vector, rec2vector);
static rec2vector findApex(rec2vector, rec2vector, rec2vector);

// FUNCTION DEFINITIONS ///////////////////////////////////////////////////////////////////////////

/*
 *
 */
void initGUI(void)
{
   // calculate size & position of acc button circle
   accBut.l =  0.50;
   accBut.r =  accBut.l + 0.40;
   accBut.t = -0.48;
   accBut.b =  accBut.t - 0.40;;

   // calculate position of clear button
   clrBut.l =  0.46;
   clrBut.r =  clrBut.l + 0.07;
   clrBut.t = -0.83;
   clrBut.b =  clrBut.t - 0.07;

   // calculate position of simulate button
   simBut.l =  0.87;
   simBut.r =  simBut.l + 0.07;
   simBut.t =  clrBut.t;
   simBut.b =  clrBut.b;

   // calculate position of track entry, centre, & exit buttons
   surrPointWithRect(trkE, trkButE, 0.1);
   surrPointWithRect(trkC, trkButC, 0.1);
   surrPointWithRect(trkX, trkButX, 0.1);

   nRacingLines = 0;
}

/*
 *
 */
void drawWindow(void)
{
   // draw x, y axes
   drawLine( 0.0, -1.0, 0.0, 1.0, GRAY);
   drawLine(-1.0,  0.0, 1.0, 0.0, GRAY);

   // draw racetrack
   drawRaceTrack(WHITE);

   // draw acc button
   double r = (accBut.r - accBut.l) / 2;

   drawCircle(accBut.l + r, accBut.t - r, r, WHITE);

   // draw bounding lines for valid angle selection
   drawArrow(accBut.l + r, accBut.b + r, angle(trkC, trkE), r, 0, BLACK);
   drawArrow(accBut.l + r, accBut.b + r, angle(trkC, trkX), r, 0, BLACK);
   
   // draw simulate button
   drawRect(simBut.l, simBut.t, simBut.r, simBut.b, WHITE);

   // draw clear button
   drawRect(clrBut.l, clrBut.t, clrBut.r, clrBut.b, WHITE);

   // draw text
   char msg[19] = "Tom McDonnell 2003";
   drawText(0.43, -0.92, msg, sizeof(msg) - 1);
}

/*
 *
 */
void processLeftClick(rec2vector mousePos)
{
   assert (-1.0 <= mousePos.x && mousePos.x <= 1.0);
   assert (-1.0 <= mousePos.y && mousePos.y <= 1.0);

   double r;          // used in case acc
   line lineE, lineX; // used in case sim

   // return to normal mode if modifying corner
   if (mouseMoveMode != normal)
   {
      mouseMoveMode = normal;
      drawRaceTrack(WHITE);
      return;
   }

   // test for button clicks
   enum {clr, sim, acc, trkEb, trkCb, trkXb, none} butPressed;
        if (insideRect(mousePos, clrBut )) butPressed = clr;
   else if (insideRect(mousePos, simBut )) butPressed = sim;
   else if (insideRect(mousePos, accBut )) butPressed = acc;
   else if (insideRect(mousePos, trkButE)) butPressed = trkEb;
   else if (insideRect(mousePos, trkButC)) butPressed = trkCb;
   else if (insideRect(mousePos, trkButX)) butPressed = trkXb;
   else                                    butPressed = none;

   // deal with button clicks
   switch (butPressed)
   {
    case clr:
      drawRect(-1.0, -1.0, 1.0, 1.0, BLACK);
      drawWindow();
      nRacingLines = 0;
      break;

    case sim:
      drawRect(-1.0, -1.0, 1.0, 1.0, BLACK);
      drawWindow();
      drawRect(simBut.l, simBut.t, simBut.r, simBut.b, RED); // indicate button clicked
      syncRacingLines(rL, nRacingLines);
      plotRacingLines(rL, nRacingLines);
      break;

    case acc:
      if (nRacingLines < maxRacingLines)
      {
         r = (accBut.r - accBut.l) / 2;
         rL[nRacingLines].setF(atan2(mousePos.y - (accBut.t - r), mousePos.x - (accBut.l + r)));

         drawArrow(accBut.l + r, accBut.t - r, rL[nRacingLines].getF(),
                   r, 0.05, getColor(nRacingLines)                     );

         lineE = findLineEquation(trkE, trkC);
         lineX = findLineEquation(trkC, trkX);
         trkA  = findApex(trkE, trkC, trkX);

         if (rL[nRacingLines].fitToCorner(lineE, lineX, trkA))
         {
            rL[nRacingLines].plot(getColor(nRacingLines));
            ++nRacingLines;
         }
      }
      break;

    case trkEb:
      if (mouseMoveMode != moveTrkE)
      {
         mouseMoveMode = moveTrkE;
         drawRect(-1.0, -1.0, 1.0, 1.0, BLACK);
         drawWindow();
         nRacingLines = 0;
      }
      break;

    case trkCb:
      if (mouseMoveMode != moveTrkC)
      {
         mouseMoveMode = moveTrkC;
         drawRect(-1.0, -1.0, 1.0, 1.0, BLACK);
         drawWindow();
         nRacingLines = 0;
      }

      break;

    case trkXb:
      if (mouseMoveMode != moveTrkX)
      {
         mouseMoveMode = moveTrkX;
         drawRect(-1.0, -1.0, 1.0, 1.0, BLACK);
         drawWindow();
         nRacingLines = 0;
      }
      break;
   }
}

/*
 *
 */
void processMouseMove(rec2vector mousePos)
{
   assert (-1.0 <= mousePos.x && mousePos.x <= 1.0);
   assert (-1.0 <= mousePos.y && mousePos.y <= 1.0);

   switch (mouseMoveMode)
   {
    case moveTrkE:
      drawLine(trkE, trkC, BLACK);                     // erase old track corner entry segment
      drawPixel(trkA, BLACK);
      trkE = lineIntersectScrBoundary(trkC, mousePos); // update track screen entry point
      surrPointWithRect(trkE, trkButE, 0.1);           // update track screen entry point button
      drawWindow();
      break;

    case moveTrkC:
      drawLine(trkE, trkC, BLACK);           // erase old track corner entry segment
      drawLine(trkC, trkX, BLACK);           // erase old track corner exit  segment
      drawPixel(trkA, BLACK);
      trkC = mousePos;                       // update track corner centre point
      surrPointWithRect(trkC, trkButC, 0.1); // update track corner centre point button
      drawWindow();
      break;

    case moveTrkX:
      drawLine(trkC, trkX, BLACK);                     // erase old track corner exit segment
      drawPixel(trkA, BLACK);
      trkX = lineIntersectScrBoundary(trkC, mousePos); // update track screen exit point
      surrPointWithRect(trkX, trkButX, 0.1);           // update track screen exit point button
      drawWindow();
      break;

    case normal:
      // do nothing
      break;
   }
}

// STATIC FUNCTION DEFINITIONS ////////////////////////////////////////////////////////////////////

/*
 *
 */
static void drawRaceTrack(COLORREF c)
{
                        // points where:
   rec2vector OEoffScr, // outside border line  of entry track meets screen boundary
              OXoffScr, // outside border line  of exit  track meets screen boundary
              IEoffScr, //  inside border line  of entry track meets screen boundary
              IXoffScr, //  inside border line  of exit  track meets screen boundary
              OEXmeet,  // outside border lines of entry and exit meet
              IEXmeet;  //  inside border lines of entry and exit meet (corner apex)

   // find apex of corner
   trkA = findApex(trkE, trkC, trkX);

   drawPixel(trkA, c);

   // find point where outside border line of entry track meets screen boundary
   
   // find point where outside border line of exit  track meets screen boundary

   // find point where inside  border line of entry track meets screen boundary

   // find point where inside  border line of exit  track meets screen boundary

   // find point where outside border lines of entry and exit meet

   drawLine(trkE, trkC, c);
   drawLine(trkC, trkX, c);

   //drawLine(IEXmeet, IEoffScr, c);
   //drawLine(IEXmeet, IXoffScr, c);
   //drawLine(OEXmeet, OEoffScr, c);
   //drawLine(OEXmeet, OXoffScr, c);
}

/*
 *
 */
static bool insideRect(rec2vector p, rect r)
{
   if (   r.l < p.x && p.x < r.r
       && r.b < p.y && p.y < r.t) return true;
   else                           return false;
}

/*
 *
 */
static void surrPointWithRect(rec2vector p, rect &r, double l)
{
   r.l = p.x - l;
   r.r = p.x + l;
   r.t = p.y + l;
   r.b = p.y - l;
}

/*
 * Find point where line segment begining at p1 and passing through p2
 * intersects with the screen boundary.
 */
static rec2vector lineIntersectScrBoundary(rec2vector p1, rec2vector p2)
{
   line lineT(0.0,  1.0, true ), // top    boundary line (y = 0x + 1)
        lineB(0.0, -1.0, true ), // bottom boundary line (y = 0x - 1)
        lineL(0.0, -1.0, false), // left   boundary line (x = 0y - 1)
        lineR(0.0,  1.0, false), // right  boundary line (x = 0y + 1)
        line;

   // find direction of line segment
   double angle = atan2(p2.y - p1.y, p2.x - p1.x);

   // find equation representation of line
   line = findLineEquation(p1, p2);

   // find which sides to test for intersection
   enum {TL, TR, BL, BR} sides;
        if (-pi     < angle && angle <= -pi / 2) sides = BL;
   else if (-pi / 2 < angle && angle <=  0     ) sides = BR;
   else if ( 0      < angle && angle <=  pi / 2) sides = TR;
   else if ( pi / 2 < angle && angle <=  pi    ) sides = TL;

   // find intersection point
   rec2vector p;
   switch (sides)
   {
    case BL:
      p = intersection(line, lineB);
      if (!(-1.0 <= p.x && p.x <= 1.0))
        p = intersection(line, lineL);
      break;

    case BR:
      p = intersection(line, lineB);
      if (!(-1.0 <= p.x && p.x <= 1.0))
        p = intersection(line, lineR);
      break;

    case TR:
      p = intersection(line, lineT);
      if (!(-1.0 <= p.x && p.x <= 1.0))
        p = intersection(line, lineR);
      break;

    case TL:
      p = intersection(line, lineT);
      if (!(-1.0 <= p.x && p.x <= 1.0))
        p = intersection(line, lineL);
      break;
   }

   return p;
}

/*
 *
 */
static rec2vector findApex(rec2vector e, rec2vector c, rec2vector x)
{
   rec2vector p;

   p.x = (e.x + 6 * c.x + x.x) / 8;
   p.y = (e.y + 6 * c.y + x.y) / 8;

   return p;
}

/*****************************************END*OF*FILE*********************************************/
