/*************************************************************************************************\
*                                                                                                 *
* "geometry.h" -                                                                                  *
*                                                                                                 *
*       Author - Tom McDonnell 2003                                                               *
*                                                                                                 *
\*************************************************************************************************/

#ifndef GEOMETRY_H
#define GEOMETRY_H

// GLOBAL VARIABLES ///////////////////////////////////////////////////////////////////////////////

const double pi = 3.141592654;

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////

#include <TomsLibrary/vector.h>

// TYPE DEFINIIONS ////////////////////////////////////////////////////////////////////////////////

/*
 * If YeqMxPlusC is true,  line is defined by y = mx + c.
 *                  false,                    x = my + c.
 * This is done to avoid floating point errors as m approaches
 * infinity.  To minimise floating point errors, and for uniformity,
 * lines should always be defined so that m falls within range (-1, 1).
 */
class line
{
 public:
   line(double m1 = 0, double c1 = 0, bool b = true) {m = m1; c = c1; YeqMxPlusC = b;}

   void set(double m1, double c1, bool b) {m = m1; c = c1; YeqMxPlusC = b;}
   double m, c;
   bool YeqMxPlusC;
};

#endif

// FUNCTION DECLARATIONS //////////////////////////////////////////////////////////////////////////

double      angle(rec2vector, rec2vector);
line        findLineEquation(rec2vector, rec2vector);
rec2vector  intersection(line, line);

bool operator>(rec2vector, line);
bool operator>(line, rec2vector);
bool operator<(rec2vector, line);
bool operator<(line, rec2vector);

/*****************************************END*OF*FILE*********************************************/
