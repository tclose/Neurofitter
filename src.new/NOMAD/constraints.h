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

/* constraints.h -- class definition for Constraints. This class manages the
   bound constraints. */

#ifndef _CONSTRAINTS_H_
#define _CONSTRAINTS_H_
#include <fstream>
using namespace std;

class Constraints
{
  /* Constraints has the following data structure:
     - an array of real numbers representing the lower bounds.
     - an array of real numbers representing the upper bounds.
     - an array of real numbers, 'epsilon_array', is a region of space inside
       the bounds. If the current point is within an epsilon value of a
       boundary, we have to change the poll directions.
     - an array of integers, 'proximity_array', indicates wether the current
       point is outside the bound, inside within epsilon of the bound or well
       inside the bound. */
 private:
  double * lower, * upper, * epsilon_array;
  int * proximity_array;
  unsigned int display_factor;

// The class methods are described in "constraints.cpp".
  void showEpsilonArray(const unsigned int dimension);
  void showProximityArray(const unsigned int dimension);

 public:
  Constraints(const unsigned int dimension, const double * low,
	      const double * up, const double * scales_add,
	      const double * scales_mult);
  ~Constraints();
  bool check(const unsigned int dimension, const double * coord);
  double * getLowerBounds();
  double * getUpperBounds();
  int * getProximityArray();
  void buildEpsilonArray(const unsigned int dimension, const double deltaT,
			 const double delta, const double factor);
  void continueRun();
  void setDisplay(const unsigned int display);
  void showInput(const unsigned int dimension, ofstream & fout,
		 const double * scales_add, const double * scales_mult);
  void updateProximityArray(const unsigned int dimension,
			    const double * coord);
};
#endif
