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

// constraints.cpp -- Constraints class methods.

#include <cmath>
#include <iostream>
using namespace std;
#include "constraints.h"
#include "defines.h"

/* Private methods:
******************************************************************************/

// This method outputs the epsilon array.
void Constraints::showEpsilonArray(const unsigned int dimension)
{
  if (epsilon_array != NULL)
    {
      cout << "Epsilon array: (";
      for (unsigned int i = 0; i < (dimension - 1); i++)
	cout << epsilon_array[i] << ", ";
      cout << epsilon_array[dimension - 1] << ")\n";
    }
}

// This method outputs the proximity array.
void Constraints::showProximityArray(const unsigned int dimension)
{
  if (proximity_array != NULL)
    {
      cout << "Proximity array: (";
      for (unsigned int i = 0; i < (dimension - 1); i++)
	cout << proximity_array[i] << ", ";
      cout << proximity_array[dimension - 1] << ")\n";
    }
}

/* Public methods:
******************************************************************************/

/* Default constructor. This object is created by Evaluator when the problem
   has bound constraints. */
Constraints::Constraints(const unsigned int dimension, const double * low,
			 const double * up, const double * scales_add,
			 const double * scales_mult)
{
  display_factor = 0;
  // The constraint arrays are built.
  lower = new double[dimension];
  upper = new double[dimension];
  // These arrays are null at the start.
  epsilon_array = NULL;
  proximity_array = NULL;
  // The constraint arrays are initialized.
  for (unsigned int i = 0; i < dimension; i++)
    {
      // 'lower' and 'upper' are scaled as the the starting point is.
      lower[i] = (low[i] / scales_mult[i]) + scales_add[i];
      upper[i] = (up[i] / scales_mult[i]) + scales_add[i];
    }
}

// Destructor. The arrays are destroyed.
Constraints::~Constraints()
{
  if (epsilon_array != NULL)
    delete [] epsilon_array;
  delete [] lower;
  delete [] upper;
  if (proximity_array != NULL)
    delete [] proximity_array;
}

/* This method checks if the point at the current iteration is outside or
   inside the bounds. It returns true if it's inside, false otherwise. */
bool Constraints::check(const unsigned int dimension, const double * coord)
{
  // Each coordinate has to be inside the bounds.
  for (unsigned int i = 0; i < dimension; i++)
    {
      // If the coordinate is outside the bounds...
      if ((coord[i] < lower[i]) || (coord[i] > upper[i]) )
	// the method will return 'false'.
	return false;
    }
  return true;
}

// This method returns the lower bounds array.
double * Constraints::getLowerBounds()
{
  return lower;
}

// This method returns the upper bounds array.
double * Constraints::getUpperBounds()
{
  return upper;
}

// This method returns the proximity array.
int * Constraints::getProximityArray()
{
  return proximity_array;
}

/* This method calculates the tolerances within which the constraints are
   considered active. It's done only once at the start of the problem
   resolution, with the mesh size, the mesh size termination criterion and the
   mesh factor. */
void Constraints::buildEpsilonArray(const unsigned int dimension,
				    const double deltaT, const double delta,
				    const double factor)
{
  double bound_diff, diff;
  // The various array are built.
  if (epsilon_array == NULL)
    epsilon_array = new double[dimension];
  if (proximity_array == NULL)
    proximity_array = new int[dimension];
  // The epsilon values for each dimension are calculated in this loop.
  for (unsigned int i = 0; i < dimension; i++)
    {
      // The first part works even when the bounds are not finite.
      // If the mesh size is a termination criterion...
      if (deltaT > 0.0)
	// we multiply it by 4 (arbitrary value).
	epsilon_array[i] = 4 * deltaT;
      else   // mesh size is not a termination criterion.
	// epsilon_array[i] =  (starting) mesh size * (mesh factor ^ -5).
	epsilon_array[i] = delta * pow(factor, -5.0);

      // The second part works for finite bounds.
      // We check if the bounds are finite.
      if ((lower[i] != -INFINIT) && (upper[i] != INFINIT) )
	{
	  /* We calculate a value relative to the lower and upper bounds, and
	     put it in 'diff'. */
	  bound_diff = upper[i] - lower[i];
	  diff = bound_diff * 0.01;
	  /* If this value is greater than the one calculated in the first
	     part, we keep it. */
	  if (diff > epsilon_array[i])
	    epsilon_array[i] = diff;
	}
    }
  // The epsilon array is shown.
  if (display_factor >= 5)
    showEpsilonArray(dimension);
}

// The user wants to continue the current run.
void Constraints::continueRun()
{
  // These arrays are deleted.
  if (epsilon_array != NULL)
    delete [] epsilon_array;
  if (proximity_array != NULL)
    delete [] proximity_array;
}

// This method sets the display factor.
void Constraints::setDisplay(const unsigned int display)
{
  display_factor = display;
}

// This method outputs the object's data in a file.
void Constraints::showInput(const unsigned int dimension, ofstream & fout,
			    const double * scales_add,
			    const double * scales_mult)
{
  if (!fout.is_open() )
    return;
  unsigned int i;
  // The lower bounds array is shown.
  if (lower != NULL)
    {
      fout << "Lower bounds: (";
      for (i = 0; i < (dimension - 1); i++)
	fout << (lower[i]  - scales_add[i]) * scales_mult[i] << ", ";
      fout << (lower[dimension - 1]  - scales_add[i]) * scales_mult[i];
      fout << ")\n";
    }
  // The upper bounds array is shown.
  if (upper != NULL)
    {
      fout << "Upper bounds: (";
      for (i = 0; i < (dimension - 1); i++)
	fout << (upper[i]  - scales_add[i]) * scales_mult[i] << ", ";
      fout << (upper[dimension - 1]  - scales_add[i]) * scales_mult[i];
      fout << ")\n";
    }
}

// The values of the proximity vector are updated.
void Constraints::updateProximityArray(const unsigned int dimension,
				       const double * coord)
{
  /* If the proximity array is null we do nothing; it means that the poll
     directions don't need to be updated. */
  if (proximity_array != NULL)
    {
      unsigned int i;
      if (display_factor >= 5)
	{
	  cout << "\nUpdating proximity array...\n";
	  cout << "Received point: ";
	  for (i = 0; i < dimension - 1; i++)
	    cout << coord[i] << ", ";
	  cout << coord[dimension - 1] << "\n";
	}
      // The proximity array is updated in this loop..
      for (i = 0; i < dimension; i++)
	{
	  // If the coordinate is within epsilon of the bounds...
	  if ((coord[i] - epsilon_array[i] < lower[i]) || 
	      (coord[i] + epsilon_array[i] > upper[i]) )
	    // ... we give it a '1' value.
	    proximity_array[i] = 1;
	  else   // The coordinate is farther inside than epsilon.
	    // The value is '0'.
	    proximity_array[i] = 0;
	}
      // The proximity array is shown.
      if (display_factor >= 5)
	showProximityArray(dimension);
    }
}
