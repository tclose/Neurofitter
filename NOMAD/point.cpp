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

// point.cpp -- Point class methods.

#include <iostream>
using namespace std;
#include "defines.h"
#include "point.h"

// Constructor used to build most points in the program.
Point::Point(const unsigned int dim, const double * coord,
	     const double * scales_add, const double * scales_mult)
  : dimension(dim)
{
  unsigned int i;
  coordinates = new double[dim];
  // All points created except the starting point are in this case.
  if ((scales_add == NULL) && (scales_mult == NULL))
    for (i = 0; i < dim; i++)
      coordinates[i] = coord[i];
  else   // Starting point case: we have to scale the variables.
    for (i = 0; i < dim; i++)
      coordinates[i] = (coord[i] / scales_mult[i]) + scales_add[i];
}

// Copy constructor: used to build the points in the filter.
Point::Point(const Point & p)
  : dimension(p.dimension)
{
  coordinates = new double[dimension];
  for (unsigned int i = 0; i < dimension; i++)
    coordinates[i] = p.coordinates[i];
}

// Destructor. The memory allocated for the array is freed.
Point::~Point()
{
  delete [] coordinates;
}

// This method overloads the assignment operator.
void Point::operator=(const Point & p)
{
  if (dimension != p.dimension)
    return;
  else
    {
      for (unsigned int i = 0; i < dimension; i++)
	coordinates[i] = p.coordinates[i];
    }
}

// This method makes a deep copy of a point.
void Point::update(const Point * p)
{
  if (dimension != p->dimension)
    {
      dimension = p->dimension;
      delete [] coordinates;
      coordinates = new double[dimension];
    }
  for (unsigned int i = 0; i < dimension; i++)
    coordinates[i] = p->coordinates[i];
}

// This method outputs the point on the screen.
void Point::show() const
{
  // The point is written, scaled, on the screen. This is useful for debugging.
  // It's written as follow: Coordinates: (x1, x2, ..., xn)
  cout << "Coordinates: (";
  for (unsigned int i = 0; i < dimension - 1; i++)
    cout << coordinates[i] << ", ";
  cout << coordinates[dimension - 1] << "), ";
}

// This method outputs the point in the history file.
void Point::showHistory(ofstream & fout, const double * scales_add,
			const double * scales_mult) const
{
  // Check to see if the file is open.
  if (!fout.is_open())
    return;

  // The point is written, unscaled, in the history file.
  for (unsigned int i = 0; i < dimension; i++)
    fout << (coordinates[i] - scales_add[i]) * scales_mult[i] << "\t";
}

// This method outputs the point in the result file.
void Point::showResults(ofstream & fout, const double * scales_add,
			const double * scales_mult) const
{
  // Check to see if the file is open.
  if (!fout.is_open())
    return;

  unsigned int i;
  // The point is written, unscaled, in the results file. It's written in the
  // form: (x1, x2, ..., xn)
  fout << "(";
  for (i = 0; i < dimension - 1; i++)
    fout << (coordinates[i] - scales_add[i]) * scales_mult[i]  << ", ";
  fout << (coordinates[dimension - 1] - scales_add[i]) * scales_mult[i];
  fout << ")\n";
}
