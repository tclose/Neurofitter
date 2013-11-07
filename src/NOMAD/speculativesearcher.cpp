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

// speculativesearcher.cpp -- SpeculativeSearcher class methods.

#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace std;
#include "defines.h"
#include "speculativesearcher.h"

// Public methods.
// ---------------

// Constructor.
SpeculativeSearcher::SpeculativeSearcher(const unsigned int how_many_pts,
					 const bool complete)
  : Searcher(how_many_pts, complete)
{
  previous = NULL;
}

// Destructor.
SpeculativeSearcher::~SpeculativeSearcher()
{
  if (previous != NULL)
    delete [] previous;
}

// This method is called by after a successful iteration.
bool SpeculativeSearcher::search(const unsigned int max_dim, const double,
				 const double * poll_center,
				 std::vector<AugmentedPoint *>
				 & list_of_points, Evaluator &)
{
  // This array will contain the generated point's coordinates.
  double * coord = new double[max_dim];
  // A search point is generated in this loop.
  if (display_factor >= 5)
    cout << "\nSpeculative search:\n-------------\n";
  if (display_factor >= 5)
    cout << "prev\tcurr\tcoord\n----\t----\t-----\n";
  for (unsigned int i = 0; i < max_dim; i++)
    {
      coord[i] = poll_center[i] + 3 * (poll_center[i] - previous[i]);
      if (display_factor >= 5)
	cout << previous[i] << "\t" << poll_center[i] << "\t" << coord[i] << "\n";
    }
  // The new point is added at the back of the vector.
  AugmentedPoint * ap = new AugmentedPoint(max_dim, coord);
  list_of_points.push_back(ap);
  // The array is destroyed.
  delete [] coord;
  //
  update(max_dim, poll_center);
  // The 'complete_search' variable is returned.
  return (complete_search);
}

// This method updates the 'previous' array.
void SpeculativeSearcher::update(const unsigned int max_dim,
				 const double * poll_center)
{
  if (previous == NULL)
    previous = new double[max_dim];
  for (unsigned int i = 0; i < max_dim; i++)
    previous[i] = poll_center[i];
}
