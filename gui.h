/*************************************************************************************************\
*                                                                                                 *
* "gui.h" -                                                                                       *
*                                                                                                 *
*  Author - Tom McDonnell 2003                                                                    *
*                                                                                                 *
*    Info : Coordinates are scaled so that top left of window is (x = -1, y = 1),                 *
*           and bottom right is (x = 1, y = -1).  Centre is thus (x - 0, y = 0).                  *
*           All lengths are similarly scaled.  This is for compatability with graphics code.      *
*                                                                                                 *
\*************************************************************************************************/

#ifndef GUI_H
#define GUI_H

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////

#include <TomsLibrary/vector.h>

// TYPE DECLARATIONS //////////////////////////////////////////////////////////////////////////////

typedef struct
{
   double l, r, t, b; // x left, x right, y top, y bottom;
} rect;

// FUNCTION DECLARATIONS //////////////////////////////////////////////////////////////////////////

void  initGUI(void);
void  drawWindow(void);
void  processLeftClick(rec2vector);
void  processMouseMove(rec2vector);

#endif

/*****************************************END*OF*FILE*********************************************/
