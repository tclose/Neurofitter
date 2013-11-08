/******************************************************************************
* NOMAD -  Nonlinear Optimization for Mixed vAriables and Derivatives.        *
* Copyright (C) 2001-2004  Gilles Couture - Ecole Polytechnique,              *
*                          Charles Audet - Ecole Polytechnique,               *
*		           John Dennis - Rice University.                     *
*                                                                             *
* Author: Gilles Couture                                                      *
* École Polytechnique de Montréal - GERAD                                     *
* C.P. 6079, Succ. Centre-ville, Montréal (Québec) H3C 3A7 Canada             *
* Tel:  (514) 340-6053 x6928                                                  *
* Email: Gilles.Couture@gerad.ca                                              *
* Fax: (514) 340-5665                                                         *
*                                                                             *
* This program is free software; you can redistribute it and/or               *
* modify it under the terms of the GNU General Public License                 *
* as published by the Free Software Foundation. You can find the              *
* GNU General Public License at:                                              *
*             http://www.gnu.org/copyleft/gpl.html                            *
*                                                                             *
* This program is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License           *
* along with this program; if not, write to the Free Software                 *
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. *
*                                                                             *
* You can find information on the NOMAD software at:                          *
*             http://www.gerad.ca/NOMAD/                                      *
******************************************************************************/

/* point.h -- class definition for Point. This class contains the point's
   coordinates. */

#ifndef _POINT_H_
#define _POINT_H_
#include <fstream>
using namespace std;

class Point
{
/* A Point object has the following data:
   - an integer, "dimension", is the number of coordinates the point has.
   - an array of real numbers, "coordinates", is the point's coordinates. */
 private:
  unsigned int dimension;
  double * coordinates;

  // The class methods are described in "point.cpp"
 public:
  Point(const unsigned int dim, const double * coord,
	const double * scales_add = NULL, const double * scales_mult = NULL);
  Point(const Point & p);
  ~Point();
  unsigned int getDimension() {return dimension;}
  // This method returns the point's coordinates.
  double * getPoint() {return coordinates;}
  void operator=(const Point & p);
  void update(const Point * p);
  void show() const;
  void showHistory(ofstream & fout, const double * scales_add,
		   const double * scales_mult) const;
  void showResults(ofstream & fout, const double * scales_add,
		   const double * scales_mult) const;
};
#endif
