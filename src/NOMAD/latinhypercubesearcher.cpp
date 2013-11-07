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

// latinhypercubesearcher.cpp -- LatinHypercubeSearcher class methods.

#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace std;
#include "defines.h"
#include "latinhypercubesearcher.h"

// Private methods.
// ----------------

//
double LatinHypercubeSearcher::translateToMeshCoordinate(const unsigned int j,
							 const double temp,
							 const double delta,
							 const double xj)
{
  return (xj + ((temp - left[j]) * delta) );
}

// This method decides how many points the LHS will generate.
double LatinHypercubeSearcher::generateHowManyPts(const unsigned int dim)
{
  double temp = search_pts;
  for (unsigned int i = 0; i < dim; i++)
    if (total[i] < temp)
      temp = total[i];
  if (display_factor >= 5)
    cout << "Generating " << temp << " points.\n";
  return temp;
}

// This method creates the 'left' and 'total' arrays.
void LatinHypercubeSearcher::createArrays(const unsigned int max_dim,
				const double delta, const double * poll_center)
{
  unsigned int i;
  // The arrays are destroyed.
  if (left != NULL)
    delete [] left;
  if (total != NULL)
    delete [] total;
  // These arrays are necessary to build the search points.
  /* For each dimension i, left[i] = (poll center - lower bound) / mesh size.
     So 'left[i]' is the number of possible grid points to the left of the poll
     center for dimension 'i'. */
  left = new double[max_dim];
  /* For each dimension i, right[i] = (upper bound - poll center) / mesh size.
     So 'right[i]' is the number of possible grid points to the right of the
     poll center for dimension 'i'. */
  double * right = new double[max_dim];
  /* For each dimension i, total[i] = (upper bound - lower bound) / mesh size.
     So 'total[i]' is the number of possible grid points for dimension 'i'. */
  total = new double[max_dim];
  // The three arrays are built in this loop.
  for (i = 0; i < max_dim; i++)
    {
      // If the bounds don't exist, or are infinite...
      if ((lower == NULL) ||
	  ((lower[i] == -INFINIT) && (upper[i] == INFINIT) ) )
	{
	  if (display_factor >= 7)
	    cout << "Case 1: No boundaries.\n";
	  left[i] = right[i] = search_pts;
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
      total[i] = left[i] + right[i] + 1;
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

// This method selects a point in every subspace.
void LatinHypercubeSearcher::selectPoints(const unsigned int max_dim,
					  const double nb_of_points)
{
  unsigned int i, j;
  double pi, u, v, counter, rand_nbr, z;
  if (!selected_points.empty() )
    selected_points.clear();
  std::vector<double> vec;
  if (display_factor >= 7)
    cout << "Number of points in each subspace:\n";
  /* This loop creates 'max_dim' vectors of real numbers, one for each
     dimension. */
  for (i = 0; i < max_dim; i++)
    {
      if (!vec.empty() )
	vec.clear();
      counter = 0.0;
      if (display_factor >= 7)
	cout << "Dimension " << i + 1 << ": ";
      // This loop creates a vector of real numbers, of size 'nb_of_points'.
      for (j = 1; j <= nb_of_points; j++)
	{
	  u = (j - 1) * (total[i] - 1) / nb_of_points;
	  v = j * (total[i] - 1) / nb_of_points;
	  if ((v - floor(v) ) == 0.0)
	    v = v - 1.0;
	  if (j == nb_of_points)
	    pi = floor(v) - ceil(u) + 2;
	  else
	    pi = floor(v) - ceil(u) + 1;
	  if (display_factor >= 7)
	    cout << pi << " ";
	  // A random number is generated.
	  rand_nbr = rand();
	  /* 'z' is an integer between 0 and pi
	     (rand_nbr / r_max is a number between 0 and 1). */
	  z = floor((rand_nbr / (r_max + 1) * (pi) ) );
	  vec.push_back(z + counter);
	  counter += pi;
	}
      if (display_factor >= 7)
	cout << "\n";
      selected_points.push_back(vec);
    }
  if (display_factor >= 7)
    showSelectedPoints();
}

//
void LatinHypercubeSearcher::shufflePoints(const double nb_of_points)
{
  double i, rand_nbr;
  unsigned int pos;
  std::vector<vector<double> >::iterator a_point;
  std::vector<double>::iterator a_number;
  if (!shuffled_points.empty() )
    shuffled_points.clear();
  std::vector<double> vec;
  /* The points of the selected_points vector are shuffled and put in the
     shuffled_points vector in this loop. */
  for (a_point = selected_points.begin(); a_point < selected_points.end();
       a_point++)
    {
      if (!vec.empty() )
	vec.clear();
      // This loop creates a vector of real numbers, of size 'nb_of_points'.
      for (i = nb_of_points; i > 0.0; i--)
	{
	  // A random number is generated.
	  if (i > 1.0)
	    {
	      rand_nbr = rand();
	      /* 'pos' is an integer between 0 and (nb_of_points - 1).
		 (rand_nbr / r_max is a number between 0 and 1). */
	      pos = (unsigned int)floor((rand_nbr / (r_max + 1) * i) );
	    }
	  else
	    pos = 0;
	  a_number = ((* a_point).begin() + pos);
	  vec.push_back(* a_number);
	  (* a_point).erase(a_number);
	}
      shuffled_points.push_back(vec);
    }
  if (display_factor >= 7)
    showShuffledPoints();
  if (!selected_points.empty() )
    selected_points.clear();
}

//
void LatinHypercubeSearcher::fillTheList(const unsigned int max_dim,
				const double delta, const double * poll_center,
				const double nb_of_points,
			        std::vector<AugmentedPoint *> & list_of_points)
{
  unsigned int i, j;
  double temp;
  std::vector<vector<double> >::iterator a_point;
  double * coord = new double[max_dim];
  for (i = 0; i < nb_of_points; i++)
    {
      a_point = shuffled_points.begin();
      for (j = 0; j < max_dim; j++)
	{
	  temp = (* a_point)[i];
	  coord[j] = translateToMeshCoordinate(j, temp, delta, poll_center[j]);
	  a_point++;
	}
      /* An AugmentedPoint object 'ap' is created, from the double array
	 'coord'. */
      AugmentedPoint * ap = new AugmentedPoint(max_dim, coord);
      // 'ap' is inserted at the end of the list of points.
      list_of_points.push_back(ap);
    }
  delete [] coord;
}

//
void LatinHypercubeSearcher::showSelectedPoints()
{
  unsigned int i = 0;
  std::vector<vector<double> >::iterator a_point;
  std::vector<double>::iterator a_number;
  cout << "Selected points in each subspace:\n";
  for (a_point = selected_points.begin(); a_point < selected_points.end();
       a_point++)
    {
      cout << "Dimension " << ++i << ": ";
      for (a_number = (* a_point).begin(); a_number < (* a_point).end();
	   a_number++)
	cout << (* a_number) << " ";
      cout << "\n";
    }
}

//
void LatinHypercubeSearcher::showShuffledPoints()
{
  unsigned int i = 0;
  std::vector<vector<double> >::iterator a_point;
  std::vector<double>::iterator a_number;
  cout << "Shuffled points in each subspace:\n";
  for (a_point = shuffled_points.begin(); a_point < shuffled_points.end();
       a_point++)
    {
      cout << "Dimension " << ++i << ": ";
      for (a_number = (* a_point).begin(); a_number < (* a_point).end();
	   a_number++)
	cout << (* a_number) << " ";
      cout << "\n";
    }
}

// Public methods.
// ---------------

// Constructor.
LatinHypercubeSearcher::LatinHypercubeSearcher(const unsigned int how_many_pts,
					       const bool complete,
					       Evaluator & eval)
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
LatinHypercubeSearcher::~LatinHypercubeSearcher()
{
  // The arrays are destroyed.
  if (left != NULL)
    delete [] left;
  if (total != NULL)
    delete [] total;
}

// This method is called by the Solver object at every iteration.
bool LatinHypercubeSearcher::search(const unsigned int max_dim,
				const double delta, const double * poll_center,
				std::vector<AugmentedPoint *> & list_of_points,
				Evaluator & eval)
{
  if (display_factor >= 3)
    {
      cout << "\nStart of latin hypercube search: ";
      cout << search_pts << " points, delta = ";
      cout << delta << "\n--------------------------------\nPoll center (";
      for (unsigned int i = 0; i < max_dim - 1; i++)
	cout << poll_center[i] << "  ";
      cout << poll_center[max_dim - 1] << ")\n";
      cout << "\nCurrent best iterate: ";
      eval.showIncumbent();
      cout << "\n";
    }
  /* This function creates the 'total' array: how many mesh points there are
     for each dimension. */
  createArrays(max_dim, delta, poll_center);
  // This function call returns the number of points the LHS will generate.
  double nb_of_points = generateHowManyPts(max_dim);
  /* This function selects a random point in every one of the 'nb_of_points'
     subspaces. */
  selectPoints(max_dim, nb_of_points);
  // This function shuffles the selected points randomly.
  shufflePoints(nb_of_points);
  /* This function creates the list of points that will be sent to 'Evaluator',
     from the list of shuffled points. */
  fillTheList(max_dim, delta, poll_center, nb_of_points, list_of_points);
  // The 'complete_search' variable is returned.
  return (complete_search);
}

// show -- Outputs the search.
void LatinHypercubeSearcher::showInput(ofstream & fout)
{
  if (!fout.is_open() )
    return;
  fout << "Latin hypercube search, " << search_pts << " points, ";
  Searcher::showInput(fout);
  fout << "\n";
}
