/******************************************************************************
* NOMAD -  Nonlinear Optimization for Mixed vAriables and Derivatives.        *
* Copyright (C) 2001-2004  Gilles Couture - Ecole Polytechnique,              *
*                          Charles Audet - Ecole Polytechnique,               *
*		           John Dennis - Rice University.                     *
*                                                                             *
* Author: Gilles Couture                                                      *
* �cole Polytechnique de Montr�al - GERAD                                     *
* C.P. 6079, Succ. Centre-ville, Montr�al (Qu�bec) H3C 3A7 Canada             *
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

// randomsearcher.cpp -- RandomSearcher class methods.

#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace std;
#include "defines.h"
#include "randomsearcher.h"

// Private methods.
// ----------------

/* This method generates a single coordinate in unbounded space, based on the
   poll center, the dispersion factor and the mesh size. */
double RandomSearcher::generateCoordinateInUnboundedSpace(
		 const unsigned int max_dimension,
		 const double poll_center, const double delta)
{
  double r, z;
  /* This is an arbitrary function that help generate random points around the
     poll center. */
  double dispersion_factor = 2 + 1 / max_dimension;
  // The 'rand()' function generates a random number between 0 and RAND_MAX.
  r = rand();
  /* Most of the numbers generated by this operation with the 'tan(PI...)'
     multiplication are near the poll center; however some will be far. */
  z = dispersion_factor * tan(PI * (r / r_max - 0.5) );
  // We want 'z' to be an integer.
  z = floor(z + 0.5);
  return (poll_center + z * delta);
}

/* This method generates a single coordinate in bounded space, based on the
   poll center, the mesh size, 'n' is the number of mesh points in the bounded
   space, 'less_than_poll_center' the number of mesh points inferior to the
   poll center in the bounded space. */
double RandomSearcher::generateCoordinateInBoundedSpace(
				  const double poll_center, const double delta,
				  const double n,
				  const double less_than_poll_center)
{
  double z = 0.0;
  // A random number is generated.
  double r = rand();
  // 'z' is an integer between 0 and n (r/r_max is a number between 0 and 1).
  z = floor((r / r_max * n) + 0.5);
  /* If 'z' is less than the number of grid points to the left of the poll
     center ... */
  if (z <= less_than_poll_center)
    // ... it is substracted from it.
    return (poll_center - z * delta);
  else
    // ... else it's added to it.
    return (poll_center + (z - less_than_poll_center) * delta);
}

//
void RandomSearcher::createArrays(const unsigned int max_dim,
				const double delta, const double * poll_center)
{
  unsigned int i;
  // The arrays are destroyed.
  if (left != NULL)
    delete [] left;
  if (total != NULL)
    delete [] total;
  /* Supposing there are bound constraints, these arrays (total, left and
     right) will help build the search points. */
  /* For each dimension i, total[i] = (upper bound - lower bound) / mesh size.
     So 'total[i]' is the number of possible grid points for dimension 'i'. */
  total = new double[max_dim];
  /* For each dimension i, left[i] = (poll center - lower bound) / mesh size.
     So 'left[i]' is the number of possible grid points to the left of the poll
     center for dimension 'i'. */
  left = new double[max_dim];
  /* For each dimension i, right[i] = (upper bound - poll center) / mesh size.
     So 'right[i]' is the number of possible grid points to the right of the
     poll center for dimension 'i'. */
  double * right = new double[max_dim];
  // Now we build the three arrays.
  for (i = 0; i < max_dim; i++)
    {
      /* If the bounds are not finite, we store '0' in the arrays
         (it's an unbounded case). */
      if ((lower == NULL) ||
	  ((lower[i] == -INFINIT) && (upper[i] == INFINIT) ) )
	{
	  if (display_factor >= 7)
	    cout << "Case 1: No boundaries.\n";
	  left[i] = right[i] = 0.0;
	}
      else   // If the lower bound is infinite...
	if (lower[i] == -INFINIT)
	  {
	    if (display_factor >= 7)
	      cout << "Case 2: Infinite lower bound.\n";
	    left[i] = search_pts;
	    right[i] = floor((upper[i] - poll_center[i]) / delta);
	  }
	else   // If the upper bound is infinite...
	  if (upper[i] == INFINIT)
	    {
	      if (display_factor >= 7)
		cout << "Case 3: Infinite upper bound.\n";
	      left[i] = floor((poll_center[i] - lower[i]) / delta);
	      right[i] = search_pts;
	    }
	  else   // The bounds are finite.
	    {
	      if (display_factor >= 7)
		cout << "Case 4: Finite bounds.\n";
	      left[i] = floor((poll_center[i] - lower[i]) / delta);
	      right[i] = floor((upper[i] - poll_center[i]) / delta);
	    }
      total[i] = left[i] + right[i];
      if (display_factor >= 7)
	{
	  cout << "left[" << i << "] = " << left[i] << ", ";
	  cout << "right[" << i << "] = " << right[i] << ", ";
	  cout << "total[" << i << "] = " << total[i] << "\n";
	}
    }
  // Once the 'total' array is built, we have no more need of 'right'.
  delete [] right;
}

// This method decides how many points the random search will generate.
double RandomSearcher::generateHowManyPts(const unsigned int dim)
{
  double temp = 1.0;
  for (unsigned int i = 0; i < dim; i++)
    temp *= total[i];
  if ((temp > search_pts) || (temp == 0.0) )
    temp = search_pts;
  if (display_factor >= 5)
    cout << "Generating " << temp << " points.\n";
  return temp;
}

// This method generates the search points.
void RandomSearcher::fillTheList(const unsigned int max_dim,
				const double delta, const double * poll_center,
				const double nb_of_points,
			        std::vector<AugmentedPoint *> & list_of_points)
{
  // Various variables we'll need.
  unsigned int i;
  double value;
  // 'pts_in_list' counts all points generated by the search.
  unsigned int pts_in_list = 0;
  // This array will contain the generated point's coordinates.
  double * coord = new double[max_dim];
  // Main loop: we put 'nb_of_points' points in the list.
  while (pts_in_list < nb_of_points)
    {
      // A search point is generated in this loop.
      for (i = 0; i < max_dim; i++)
	{
	  /* First case: No bound constraints, or the bounds are infinite for
	     dimension 'i'. */
	  if ((lower == NULL) || (total[i] == 0.0) )
	    value = generateCoordinateInUnboundedSpace(max_dim,
						       poll_center[i], delta);
	  else
	    {
	      // The bounds are finite for dimension 'i'.
	      value = generateCoordinateInBoundedSpace(poll_center[i], delta,
						       total[i], left[i]);
	    }
	  // The new 'value' is put in the array.
	  coord[i] = value;
	}
      // A point was generated: the counter is incremented.
      pts_in_list++;
      // The new point is added at the back of the vector.
      AugmentedPoint * ap = new AugmentedPoint(max_dim, coord);
      list_of_points.push_back(ap);
    }
  // The array is destroyed.
  delete [] coord;
}

// Public methods.
// ---------------

// Constructor.
RandomSearcher::RandomSearcher(const unsigned int how_many_pts,
			       const bool complete, Evaluator & eval)
  : Searcher(how_many_pts, complete)
{
  // 'r_max' contains the largest possible random number generated.
  r_max = RAND_MAX;
  // If there are bound constraints...
  if (eval.getConstraints() )
    {
      // ...we point to them with 'lower' and 'upper'.
      lower = eval.getLowerBounds();
      upper = eval.getUpperBounds();
    }
  else   // There are no bound constraints.
    {
      lower = NULL;
      upper = NULL;
    }
  // The arrays are NULL at the start.
  left = NULL;
  total = NULL;
}

// Destructor.
RandomSearcher::~RandomSearcher()
{
  // The arrays are destroyed.
  if (left != NULL)
    delete [] left;
  if (total != NULL)
    delete [] total;
}

// This method is called by the Solver object at every iteration.
bool RandomSearcher::search(const unsigned int max_dim,
			    const double delta, const double * poll_center,
			    std::vector<AugmentedPoint *> & list_of_points,
			    Evaluator & eval)
{
  if (display_factor >= 3)
    {
      cout << "\nStart of random search: " << search_pts << " points, ";
      cout << "delta = "<< delta << "\n-----------------------\nPoll center (";
      for (unsigned int i = 0; i < max_dim - 1; i++)
	cout << poll_center[i] << "  ";
      cout << poll_center[max_dim - 1] << ")\n";
      cout << "\nCurrent best iterate: ";
      eval.showIncumbent();
      cout << "\n";
    }
  // 
  createArrays(max_dim, delta, poll_center);
  /* This function call returns the number of points the random search will
     generate. */
  double nb_of_points = generateHowManyPts(max_dim);
  /* This function creates the list of points that will be sent to 'Evaluator',
     from the list of shuffled points. */
  fillTheList(max_dim, delta, poll_center, nb_of_points, list_of_points);
  // The 'complete_search' variable is returned.
  return (complete_search);
}

// showInput -- Outputs the search's variables in a file.
void RandomSearcher::showInput(ofstream & fout)
{
  if (!fout.is_open() )
    return;
  fout << "Random search, " << search_pts << " points, ";
  Searcher::showInput(fout);
  fout << "\n";
}