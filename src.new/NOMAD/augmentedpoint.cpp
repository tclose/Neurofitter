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

// augmentedpoint.cpp -- AugmentedPoint class methods.

#include <fstream>
#include <iostream>
using namespace std;
#include "augmentedpoint.h"

// Initializing the static class members.
unsigned int AugmentedPoint::output_dimension = 0;
const unsigned int * AugmentedPoint::index = NULL;

// Public methods.
// ---------------

// This constructor is used at Start-up to build the starting point.
AugmentedPoint::AugmentedPoint(const unsigned int dim, const unsigned int out,
			       const unsigned int * out_index, double * coord,
			       const double * scales_add,
			       const double * scales_mult)
{
  // The Point object is created (with the correct coordinates).
  p = new Point(dim, coord, scales_add, scales_mult);
  // The static variables are set.
  output_dimension = out;
  index = out_index;
  // The outputs are set to INFINITY.
  output_values = new double[output_dimension];
  for (unsigned int i = 0; i < output_dimension; i++)
    output_values[i] = INFINIT;
  Hx = 0.0;
}

// This constructor is used to build the points for the Poll and Search.
AugmentedPoint::AugmentedPoint(const unsigned int dim, double * coord)
{
  // The Point object is created (with the correct coordinates).
  p = new Point(dim, coord);
  // All outputs are set to INFINITY.
  output_values = new double[output_dimension];
  for (unsigned int i = 0; i < output_dimension; i++)
    output_values[i] = INFINIT;
  Hx = 0.0;
}

// Copy constructor: used to build the list of points in the filter.
AugmentedPoint::AugmentedPoint(const AugmentedPoint & ap)
{
  p = new Point(* ap.p);
  output_values = new double[output_dimension];
  for (unsigned int i = 0; i < output_dimension; i++)
    output_values[i] = ap.output_values[i];
  Hx = ap.Hx;
}

// Destructor. The memory allocated for the two arrays is freed.
AugmentedPoint::~AugmentedPoint()
{
  delete p;
  delete [] output_values;
}

//
int AugmentedPoint::getPosition(const unsigned int val) const
{
  for (unsigned int i = 0; i < output_dimension; i++)
    if (index[i] == val)
      return i;
  return (-1);
}

// This method returns the point's surrogate function value.
double AugmentedPoint::getLow() const
{
  int pos = getPosition(Sx);
  if (pos >= 0)
    return output_values[pos];
  else
    return INFINIT;
}

double AugmentedPoint::getHx()
{
  return Hx;
}

// This method sets the point's low value.
void AugmentedPoint::setLow(const double val)
{
  int pos = getPosition(Sx);
  if (pos >= 0)
    output_values[pos] = val;
}

//
void AugmentedPoint::setHx(const double val)
{
  Hx = val;
}

// This method overloads the assignment operator.
void AugmentedPoint::operator=(const AugmentedPoint & ap)
{
  if (output_dimension != ap.output_dimension)
    return;
  else
    {
      for (unsigned int i = 0; i < output_dimension; i++)
	output_values[i] = ap.output_values[i];
      Hx = ap.Hx;
      * p = * ap.p;
    }
}

// This method makes a deep copy of a point.
void AugmentedPoint::update(const AugmentedPoint * ap)
{
  p->update(ap->p);
  for (unsigned int i = 0; i < output_dimension; i++)
    output_values[i] = ap->output_values[i];
  Hx = ap->Hx;
}

// Outputs the augmented point's data.
void AugmentedPoint::show() const
{
  if (p != NULL)
    p->show();
  cout << "output vector: ";
  for (unsigned int i = 0; i < output_dimension; i++)
    {
      if (output_values[i] == INFINIT)
	cout << "INFINITY   ";
      else
	cout << output_values[i] << "   ";
    }
  if (getPosition(Gx) != -1)
    {
      if (Hx == INFINIT)
	cout << "INFINITY";
      else
	cout << Hx;
    }
  cout << "\n";
}

// This method outputs the augmented point in the history file.
void AugmentedPoint::showHistory(ofstream & fout, const double * scales_add,
				 const double * scales_mult) const
{
  // Check to see if the file is open.
  if (!fout.is_open())
    return;

  // The point's coordinates are written in the history file.
  p->showHistory(fout, scales_add, scales_mult);

  // The point's outputs are written in the history file.
  for (unsigned int i = 0; i < output_dimension; i++)
    {
      if (output_values[i] == INFINIT)
	fout << "INFINITY" << "\t";
      else
	fout << output_values[i] << "\t";
    }

  // The h(x) value is written in the history file (if there are general
  // constraints in the problem).
  if (getPosition(Gx) != -1)
    (Hx == INFINIT) ? fout << "INFINITY" : fout << Hx;

  // The augmented point is written: the next point will be written on a new
  // line.
  fout << "\n";
}

// Outputs the augmented point in the results file.
void AugmentedPoint::showResults(ofstream & fout, const double * scales_add,
				 const double * scales_mult) const
{
  if (!fout.is_open())
    return;
  if (output_values[0] == INFINIT)
    fout << "INFINITY   ";
  else
    fout << output_values[0] << "   ";
  if (p != NULL)
    p->showResults(fout, scales_add, scales_mult);
}
